#pragma once

#include "opengl_functions.hxx"
#include <abstract_engine.hxx>

#include <glad/glad.h>

namespace SDLEngine
{

class Instance : public Engine::Instance<Instance>
{
public:
    template <typename... Args> void initialize_impl(Args&&... args)
    {
        if (!GL::init_sdl())
        {
            return ;
        }

        window_ = GL::create_window("Test", 1000, 1000);
        if (!window_)
        {
            SDL_Quit();
            return ;
        }

        context_ = GL::create_opengl_context(window_);
        if (!context_)
        {
            SDL_DestroyWindow(window_);
            SDL_Quit();
            return ;
        }

        if (!GL::load_opengl_functions() || !GL::is_opengl_version_supported())
        {
            SDL_GL_DeleteContext(context_);
            SDL_DestroyWindow(window_);
            SDL_Quit();
            return ;
        }

        compile_shaders();
    }


    template <typename... Args>
    void render_impl(const GLfloat vertices[], long vertices_size, Args&&... args)
    {
        GLint mousePosUniformLoc = glGetUniformLocation(program_id_, "u_mousePos");
        float mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        glUniform2f(mousePosUniformLoc, mouseX, mouseY);


        // Update the vertex buffer with the new vertices
        glBindBuffer(GL_ARRAY_BUFFER, VBO_);
        GL::listen_opengl_errors();
        glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_DYNAMIC_DRAW);
        GL::listen_opengl_errors();

        // Re-bind the VAO after updating the vertex buffer
        glBindVertexArray(VAO_);
        GL::listen_opengl_errors();

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)nullptr);
        GL::listen_opengl_errors();

        glEnableVertexAttribArray(0);
        GL::listen_opengl_errors();

        glBindVertexArray(0);
        GL::listen_opengl_errors();

        // Clear the screen and draw the new triangle
        glClear(GL_COLOR_BUFFER_BIT);
        GL::listen_opengl_errors();

        glUseProgram(program_id_);
        GL::listen_opengl_errors();

        glBindVertexArray(VAO_);
        GL::listen_opengl_errors();

        glDrawArrays(GL_TRIANGLES, 0, 3);
        GL::listen_opengl_errors();

        glBindVertexArray(0);
        GL::listen_opengl_errors();

        SDL_GL_SwapWindow(window_);
        GL::listen_opengl_errors();
    }

    template <typename... Args> void destroy_impl(Args&&... args)
    {
        glDeleteVertexArrays(1, &VAO_);
        GL::listen_opengl_errors();

        glDeleteBuffers(1, &VBO_);
        GL::listen_opengl_errors();

        glDeleteProgram(program_id_);
        GL::listen_opengl_errors();

        SDL_GL_DeleteContext(context_);
        SDL_DestroyWindow(window_);
        SDL_Quit();
    }

private:
    SDL_Window* window_    = nullptr;
    SDL_GLContext context_ = nullptr;
    GLuint program_id_     = 0;

    GLuint VBO_, VAO_;

    void compile_shaders();
};

} // namespace Engine
