#include "abstract_engine.hxx"
//#include "imgui_wrapper.hxx"
#include "opengl_functions.hxx"

#include "open_gl_shader.hxx"
#include "sdl_engine.hxx"

#include <fstream>
#include <gtest/gtest.h>

class my_engine : public sdl_sdk::engine
{
public:
    my_engine(const char *window_title, const int &height, const int width)
        : sdl_sdk::engine(window_title, height, width)
    {
    }

    ~my_engine() override = default;

    void initialize() override
    {
        sdl_sdk::engine::initialize();
        //ImWrapper::init_imgui(access_window(), get_context());
    }

    void render() override
    {
        sdl_sdk::engine::render();

        //ImWrapper::new_frame();

        //ImGui::ShowDemoWindow();

        //ImWrapper::render();

        sdl_sdk::engine::render();
    }

    void destroy() override
    {
        //ImWrapper::destroy();

        sdl_sdk::engine::destroy();
    }
};

class shader_component : public sdk::component
{
public:
    explicit shader_component(const char *name) : sdk::component(name)
    {
        shader_ = new OpenGLWrapper::Shader(k_vertex_shader_path_,
                                            k_fragment_shader_path_);
    }

    ~shader_component() override = default;

    void initialize() override
    {
        init_buffers();
    }

    void render() override
    {
        shader_change_daemon();

        // Update the vertex buffer with the new vertices
        glBindBuffer(GL_ARRAY_BUFFER, VBO_);

        const auto vertices = OpenGLWrapper::get_vertices_from_glsl_file(
            "test/shaders/triangle_vertex.glsl");

        glBufferData(GL_ARRAY_BUFFER, vertices.size(), vertices.data(),
                     GL_DYNAMIC_DRAW);

        // Re-bind the VAO after updating the vertex buffer
        glBindVertexArray(VAO_);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat),
                              static_cast<GLvoid *>(nullptr));

        glEnableVertexAttribArray(0);

        glBindVertexArray(0);

        // Clear the screen and draw the new triangle
        glClear(GL_COLOR_BUFFER_BIT);

        shader_->use();

        const float time    = static_cast<float>(SDL_GetTicks()) / 1000.0f;
        const GLint timeLoc = shader_->get_uniform_location("time");
        glUniform1f(timeLoc, time);

        glBindVertexArray(VAO_);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);
    }

    void destroy() override
    {
        glDeleteVertexArrays(1, &VAO_);

        glDeleteBuffers(1, &VBO_);

        if (shader_ != nullptr)
        {
            glDeleteProgram(shader_->get_program_id());
            delete shader_;
        }
    }

private:
    void init_buffers()
    {
        glGenVertexArrays(1, &VAO_);

        glGenBuffers(1, &VBO_);

        glBindVertexArray(VAO_);

        glBindBuffer(GL_ARRAY_BUFFER, VBO_);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat),
                              static_cast<GLvoid *>(nullptr));

        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
    }

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

    OpenGLWrapper::Shader *shader_ = nullptr;

    static constexpr std::string_view k_vertex_shader_path_ =
        "shaders/triangle_vertex.glsl";
    static constexpr std::string_view k_fragment_shader_path_ =
        "shaders/triangle_fragment.glsl";

    GLuint VBO_{};
    GLuint VAO_{};
};

TEST(TriangleTest, LavaLampTriangle)
{
    sdk::engine *engine = new my_engine("12", 1000, 1000);
    engine->initialize();
    sdk::component_ptr shader =
        std::make_unique<shader_component>("test_shader");
    engine->add_component(std::move(shader));

    SDL_Event event;
    while (true)
    {
        while (SDL_PollEvent(&event))
        {
            //ImWrapper::process_event(event);
            if (event.type == SDL_EVENT_QUIT)
            {
                goto cleanup;
            }
        }

        engine->render();
    }

cleanup:
    engine->destroy();
}

auto main(int argc, char **argv) -> int
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
