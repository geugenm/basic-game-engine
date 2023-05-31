#include "abstract_engine.hxx"
#include "imgui_wrapper.hxx"
#include "opengl_functions.hxx"

#include "open_gl_shader.hxx"
#include "sdl_engine.hxx"

#include <fstream>
#include <gtest/gtest.h>

class my_engine : public sdl_sdk::engine
{
public:
    ~my_engine() override = default;

    template <typename... Args> void initialize_impl(const char *window_title, const int &height,
                         const int width)
    {
        sdl_sdk::engine::initialize_impl(window_title, height, width);

        ImWrapper::init_imgui(window_, context_);

        shader_ = new OpenGLWrapper::Shader(k_vertex_shader_path_,
                                            k_fragment_shader_path_);

        init_buffers();
    }

    template <typename... Args>  void render_impl(const GLfloat vertices[], long vertices_size)
    {
        shader_change_daemon();

        // Update the vertex buffer with the new vertices
        glBindBuffer(GL_ARRAY_BUFFER, VBO_);

        glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_DYNAMIC_DRAW);

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

        ImWrapper::new_frame();

        ImGui::ShowDemoWindow();

        ImGui::Begin("Color Picker");
        static float color[3] = {1.0f, 0.0f, 0.0f};
        ImGui::ColorEdit3("Color", color);
        ImGui::End();

        const GLint colorLoc = shader_->get_uniform_location("color");
        glUniform3f(colorLoc, color[0], color[1], color[2]);

        ImWrapper::render();

        sdl_sdk::engine::render_impl();
    }

    template <typename... Args>  void destroy_impl()
    {
        glDeleteVertexArrays(1, &VAO_);

        glDeleteBuffers(1, &VBO_);

        if (shader_ != nullptr)
        {
            glDeleteProgram(shader_->get_program_id());
            delete shader_;
        }

        ImWrapper::destroy();

        sdl_sdk::engine::destroy_impl();
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

        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
    }

    OpenGLWrapper::Shader *shader_ = nullptr;

    static constexpr std::string_view k_vertex_shader_path_ =
        "shaders/triangle_vertex.glsl";
    static constexpr std::string_view k_fragment_shader_path_ =
        "shaders/triangle_fragment.glsl";

    GLuint VBO_{};
    GLuint VAO_{};
};

sdl_sdk::engine * sdl_sdk::create_instance()
{
    return new my_engine();
}

TEST(TriangleTest, LavaLampTriangle)
{
    auto engine = sdl_sdk::create_instance();
    engine->initialize("12", 1000, 1000);

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
