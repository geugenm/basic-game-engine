#include "abstract_engine.hxx"
#include "imgui_wrapper.hxx"
#include "opengl_functions.hxx"

#include "open_gl_shader.hxx"

#include "tahoma.h"

#include <fstream>
#include <gtest/gtest.h>

class MyEngine : public Engine::Instance<MyEngine>
{
public:
    ~MyEngine() override = default;

    void initialize_impl()
    {
        if (!OpenGLWrapper::init_sdl())
        {
            FAIL();
        }

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

        if (!OpenGLWrapper::load_opengl_functions() ||
            !OpenGLWrapper::is_opengl_version_supported())
        {
            SDL_GL_DeleteContext(context_);
            SDL_DestroyWindow(window_);
            SDL_Quit();
            FAIL();
        }

        ImWrapper::init_imgui(window_, context_);

        ImGuiIO *io = &ImGui::GetIO();
        ImFontConfig font_cfg;
        font_cfg.FontDataOwnedByAtlas = false;
        io->Fonts->AddFontFromMemoryTTF((void *)tahoma, sizeof(tahoma), 17.f,
                                        &font_cfg);

        shader_ = new OpenGLWrapper::Shader(k_vertex_shader_path_.data(),
                                            k_fragment_shader_path_.data());

        init_buffers();
    }

    void shader_change_daemon()
    {
        static std::time_t vertex_shader_last_modified   = 0;
        static std::time_t fragment_shader_last_modified = 0;

        bool vertex_shader_changed = OpenGLWrapper::file_has_changed(
            k_vertex_shader_path_.data(), vertex_shader_last_modified);
        bool fragment_shader_changed = OpenGLWrapper::file_has_changed(
            k_fragment_shader_path_.data(), fragment_shader_last_modified);

        if (vertex_shader_changed || fragment_shader_changed)
        {
            glDeleteProgram(shader_->get_program_id());

            delete shader_;
            shader_ = new OpenGLWrapper::Shader(k_vertex_shader_path_.data(),
                                                k_fragment_shader_path_.data());
        }
    }

    void render_impl(const GLfloat vertices[], long vertices_size)
    {
        shader_change_daemon();

        const float time = static_cast<float>(SDL_GetTicks()) / 1000.0f;
        GLint timeLoc    = glGetUniformLocation(shader_->get_program_id(), "time");
        glUniform1f(timeLoc, time);

        // Update the vertex buffer with the new vertices
        glBindBuffer(GL_ARRAY_BUFFER, VBO_);

        glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_DYNAMIC_DRAW);

        // Re-bind the VAO after updating the vertex buffer
        glBindVertexArray(VAO_);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat),
                              (GLvoid *)nullptr);

        glEnableVertexAttribArray(0);

        glBindVertexArray(0);

        // Clear the screen and draw the new triangle
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_->get_program_id());

        glBindVertexArray(VAO_);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);

        ImWrapper::new_frame();

        ImGui::ShowDemoWindow();

        ImWrapper::render();

        SDL_GL_SwapWindow(window_);
    }

    void destroy_impl()
    {
        glDeleteVertexArrays(1, &VAO_);

        glDeleteBuffers(1, &VBO_);

        glDeleteProgram(shader_->get_program_id());

        ImWrapper::destroy();

        SDL_GL_DeleteContext(context_);
        SDL_DestroyWindow(window_);
        SDL_Quit();
    }

private:
    void init_buffers()
    {
        glGenVertexArrays(1, &VAO_);

        glGenBuffers(1, &VBO_);

        glBindVertexArray(VAO_);

        glBindBuffer(GL_ARRAY_BUFFER, VBO_);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat),
                              (GLvoid *)nullptr);

        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
    }

    SDL_Window *window_    = nullptr;
    SDL_GLContext context_ = nullptr;

    OpenGLWrapper::Shader *shader_;

    static constexpr std::string_view k_vertex_shader_path_ =
        "shaders/triangle_vertex.glsl";
    static constexpr std::string_view k_fragment_shader_path_ =
        "shaders/triangle_fragment.glsl";

    GLuint VBO_, VAO_;
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