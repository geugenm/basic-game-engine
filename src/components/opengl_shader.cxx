#include "opengl_shader.hxx"

namespace sdk
{
std::string
suppl::get_json_relative_path(const std::filesystem::path &full_path)
{
    const std::string folder = full_path.parent_path().filename().string();

    const std::string file = full_path.filename().string();

    return folder + "/" + file;
}

GLint opengl_shader::get_uniform_location(const GLchar *uniform_name) const
{
    const GLint uniform_location =
        glGetUniformLocation(_program_id, uniform_name);

    if (uniform_location == -1)
    {
        throw std::invalid_argument(
            "Given uniform '" + std::string(uniform_name) + "' is not found.");
    }

    return uniform_location;
}

opengl_shader
opengl_shader::get_new_shader(const std::filesystem::path &vertex_source_path,
                              const std::filesystem::path &fragment_source_path)
{
    opengl_shader shader{._vertex_source_path   = vertex_source_path,
                         ._fragment_source_path = fragment_source_path};

#ifndef __ANDROID__ // Default filesystem is not working on android, use SDL_RW
    if (!exists(shader._vertex_source_path))
    {
        throw std::invalid_argument("Vertex shader file '" +
                                    shader._vertex_source_path.string() +
                                    "' is not found.");
    }

    if (!exists(shader._fragment_source_path))
    {
        throw std::invalid_argument("Fragment shader file '" +
                                    shader._fragment_source_path.string() +
                                    "' is not found.");
    }
#endif // __ANDROID__

    const std::string vertex_source   = shader._vertex_source_path.string();
    const std::string fragment_source = shader._fragment_source_path.string();

    GLuint vertex = opengl_subsdk::get_compiled_shader_from_file(
        GL_VERTEX_SHADER, vertex_source.data());
    GLuint fragment = opengl_subsdk::get_compiled_shader_from_file(
        GL_FRAGMENT_SHADER, fragment_source.data());

    shader._program_id = opengl_subsdk::get_new_program();

    glAttachShader(shader._program_id, vertex);
    glAttachShader(shader._program_id, fragment);

    opengl_subsdk::link_shader_program(shader._program_id);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return shader;
}

opengl_shader opengl_shader::deserialize(const nlohmann::json &input_json)
{
    return {
        ._vertex_source_path =
            resources_path.string() +
            input_json.value("vertex_source_path", "shaders/missing.vert"),
        ._fragment_source_path =
            resources_path.string() +
            input_json.value("fragment_source_path", "shaders/missing.frag"),
        ._program_id = input_json.value("program_id", GLuint{}),
    };
}

nlohmann::json opengl_shader::serialize() const
{
    return {{"vertex_source_path",
             sdk::suppl::get_json_relative_path(_vertex_source_path)},
            {"fragment_source_path",
             sdk::suppl::get_json_relative_path(_fragment_source_path)},
            {"program_id", _program_id}};
}
} // namespace sdk