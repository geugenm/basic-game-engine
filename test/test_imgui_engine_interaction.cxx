#include "abstract_engine.hxx"
#include "imgui_wrapper.hxx"
#include "opengl_functions.hxx"

#include "open_gl_shader.hxx"

#include <fstream>
#include <gtest/gtest.h>

class MyEngine : public Engine::Instance<MyEngine>
{
public:
    ~MyEngine() override = default;

    void initialize_impl()
    {
        OpenGLWrapper::init_sdl();

        window_ = OpenGLWrapper::get_new_sdl_window("Test", 1000, 1000);
        if (!window_)
        {
            SDL_Quit();
            FAIL();
        }

        context_ = OpenGLWrapper::get_new_sdl_gl_context(window_);
        if (!context_)
        {
            SDL_DestroyWindow(window_);
            SDL_Quit();
            FAIL();
        }

        OpenGLWrapper::init_opengl();

        ImWrapper::init_imgui(window_, context_);

        shader_ = new OpenGLWrapper::Shader(k_vertex_shader_path_.data(),
                                            k_fragment_shader_path_.data());

        init_buffers();
    }

    void render_impl(const GLfloat vertices[], long vertices_size)
    {
        shader_change_daemon();

        // Update the vertex buffer with the new vertices
        glBindBuffer(GL_ARRAY_BUFFER, VBO_);

        glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_DYNAMIC_DRAW);

        // Re-bind the VAO after updating the vertex buffer
        glBindVertexArray(VAO_);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat),
                              static_cast<GLvoid *>(nullptr));

        OpenGLWrapper::disable_vertex_attribute_array();

        OpenGLWrapper::unbind_vertex_array();

        // Clear the screen and draw the new triangle
        glClear(GL_COLOR_BUFFER_BIT);

        shader_->use();

        const float time = static_cast<float>(SDL_GetTicks()) / 1000.0f;
        const GLint timeLoc = shader_->get_uniform_location("time");
        glUniform1f(timeLoc, time);

        glBindVertexArray(VAO_);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        OpenGLWrapper::unbind_vertex_array();


        ImWrapper::new_frame();

        ImGui::ShowDemoWindow();

        ImWrapper::render();

        SDL_GL_SwapWindow(window_);
    }

    void destroy_impl()
    {
        glDeleteVertexArrays(1, &VAO_);

        glDeleteBuffers(1, &VBO_);

        if (shader_ != nullptr) {
            glDeleteProgram(shader_->get_program_id());
            delete shader_;
        }

        ImWrapper::destroy();

        SDL_GL_DeleteContext(context_);
        SDL_DestroyWindow(window_);
        SDL_Quit();
    }

private:
    void shader_change_daemon()
    {
        static std::time_t vertex_shader_last_modified   = 0;
        static std::time_t fragment_shader_last_modified = 0;

        const bool vertex_shader_changed = OpenGLWrapper::file_has_changed(
            k_vertex_shader_path_.data(), vertex_shader_last_modified);
        const bool fragment_shader_changed = OpenGLWrapper::file_has_changed(
            k_fragment_shader_path_.data(), fragment_shader_last_modified);

        if (vertex_shader_changed || fragment_shader_changed)
        {
            glDeleteProgram(shader_->get_program_id());

            shader_->recompile();
        }
    }

    void init_buffers()
    {
        glGenVertexArrays(1, &VAO_);

        glGenBuffers(1, &VBO_);

        glBindVertexArray(VAO_);

        glBindBuffer(GL_ARRAY_BUFFER, VBO_);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat),
                              static_cast<GLvoid *>(nullptr));

        OpenGLWrapper::disable_vertex_attribute_array();

        OpenGLWrapper::unbind_vertex_array();
    }

    SDL_Window *window_    = nullptr;
    SDL_GLContext context_ = nullptr;

    OpenGLWrapper::Shader *shader_ = nullptr;

    static constexpr std::string_view k_vertex_shader_path_ =
        "shaders/triangle_vertex.glsl";
    static constexpr std::string_view k_fragment_shader_path_ =
        "shaders/triangle_fragment.glsl";

    GLuint VBO_{};
    GLuint VAO_{};
};

template <> Engine::Instance<MyEngine> *Engine::create_instance()
{
    return new MyEngine();
}

TEST(TriangleTest, LavaLampTriangle)
{
    Engine::Instance<MyEngine> *engine = Engine::create_instance<MyEngine>();
    engine->initialize();

    SDL_Event event;
    while (true)
    {
        while (SDL_PollEvent(&event))
        {
            ImWrapper::process_event(event);
            if (event.type == SDL_EVENT_QUIT)
            {
                goto cleanup;
            }
        }

        auto vertices = OpenGLWrapper::get_vertices_from_glsl_file(
            "test/shaders/triangle_vertex.glsl");

        engine->render(vertices.data(), vertices.size());
    }

cleanup:
    engine->destroy();
}

auto main(int argc, char **argv) -> int
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
