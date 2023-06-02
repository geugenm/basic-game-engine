#include "abstract_engine.hxx"
#include "imgui_wrapper.hxx"
#include "opengl_functions.hxx"

#include "open_gl_shader.hxx"
#include "sdl_engine.hxx"

#include <fstream>
#include <gtest/gtest.h>

class shader_component : public sdk::component
{
public:
    explicit shader_component(const char *name) : sdk::component(name)
    {
        shader_ = new opengl_subsdk::shader(k_vertex_shader_path_,
                                            k_fragment_shader_path_);
    }

    ~shader_component() override = default;

    void initialize_impl() override
    {
        init_buffers();
    }

    void render_impl() override
    {
        shader_change_daemon();

        // Update the vertex buffer with the new vertices
        glBindBuffer(GL_ARRAY_BUFFER, VBO_);

        const auto vertices = opengl_subsdk::get_vertices_from_glsl_file(
            "test/shaders/triangle_vertex.glsl");

        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size()),
                     vertices.data(), GL_DYNAMIC_DRAW);

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

    void destroy_impl() override
    {
        glDeleteVertexArrays(1, &VAO_);

        glDeleteBuffers(1, &VBO_);

        if (shader_ != nullptr)
        {
            glDeleteProgram(shader_->get_program_id());
            delete shader_;
        }
    }

    [[nodiscard]] opengl_subsdk::shader *get_shader()
    {
        return shader_;
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

        const bool vertex_shader_changed = opengl_subsdk::file_has_changed(
            k_vertex_shader_path_.data(), vertex_shader_last_modified);
        const bool fragment_shader_changed = opengl_subsdk::file_has_changed(
            k_fragment_shader_path_.data(), fragment_shader_last_modified);

        if (vertex_shader_changed || fragment_shader_changed)
        {
            glDeleteProgram(shader_->get_program_id());

            shader_->recompile();
        }
    }

    opengl_subsdk::shader *shader_ = nullptr;

    static constexpr std::string_view k_vertex_shader_path_ =
        "shaders/triangle_vertex.glsl";
    static constexpr std::string_view k_fragment_shader_path_ =
        "shaders/triangle_fragment.glsl";

    GLuint VBO_{};
    GLuint VAO_{};
};

class imgui_component : public sdk::component
{
public:
    explicit imgui_component(SDL_Window *sdl_window, SDL_GLContext sdl_context,
                             const char *name = "imgui")
        : sdk::component(name)
    {
        imgui_subsdk::init_imgui(sdl_window, sdl_context);
    }

    ~imgui_component() override = default;

    void initialize_impl() override {
        for (auto const &window : windows_)
        {
            window->initialize();
        }
    }

    void render_impl() override
    {
        imgui_subsdk::new_frame();

        for (auto const &window : windows_)
        {
            window->render();
        }

        ImGui::ShowDemoWindow();

        imgui_subsdk::render();
    }

    void destroy_impl() override
    {
        for (auto const &window : windows_)
        {
            window->destroy();
        }

        imgui_subsdk::destroy();
    }

    void add_window(std::unique_ptr<sdk::component> imgui_window)
    {
        if (imgui_window == nullptr)
        {
            throw sdk::engine_error("Trying to add null component",
                                    "add_component");
        }

        imgui_window->initialize();
        windows_.push_back(std::move(imgui_window));
    }

private:
    std::vector<std::unique_ptr<sdk::component>> windows_;
};

class imgui_shader_editor : public sdk::component
{
public:
    imgui_shader_editor(const imgui_component &imgui_main,
                        opengl_subsdk::shader *shader,
                        const char *name = "imgui_shader_editor")
        : sdk::component(name), imgui_(imgui_main), shader_(shader)
    {
        if (shader_ == nullptr)
        {
            throw sdk::engine_error("Given shader is null.");
        }
    }

    void initialize_impl() override
    {
        if (!imgui_.is_initialized())
        {
            throw sdk::engine_error("ImGui component is not initialized");
        }
    }

    void render_impl() override
    {
        ImGui::Begin("Color Picker");
        static float color[3] = {1.0f, 0.0f, 0.0f};
        ImGui::ColorEdit3("Color", color);
        ImGui::End();

        const GLint colorLoc = shader_->get_uniform_location("color");
        glUniform3f(colorLoc, color[0], color[1], color[2]);
    }

    void destroy_impl() override {}

private:
    const imgui_component &imgui_;
    opengl_subsdk::shader *shader_;
};

TEST(TriangleTest, LavaLampTriangle)
{
    auto *engine = new sdl_subsdk::engine("12", 1000, 1000);
    engine->initialize();


    auto shader = std::make_unique<shader_component>("test_shader");

    auto imgui = std::make_unique<imgui_component>(engine->get_window(),
                                                   engine->get_context());
    imgui->initialize();

    auto shader_editor = std::make_unique<imgui_shader_editor>(
        *imgui.get(), shader->get_shader());

    imgui->add_window(std::move(shader_editor));

    engine->add_component(std::move(shader));
    engine->add_component(std::move(imgui));




    SDL_Event event;
    while (true)
    {
        while (SDL_PollEvent(&event))
        {
            imgui_subsdk::process_event(event);
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
