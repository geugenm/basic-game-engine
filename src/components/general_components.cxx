#include "general_components.hxx"

namespace sdk
{

glm::vec3 util::glm_vec3_from_json(const nlohmann::json &j,
                                   const glm::vec3 &default_value)
{
    return {
        j.value("x", default_value.x),
        j.value("y", default_value.y),
        j.value("z", default_value.z),
    };
}

nlohmann::json util::glm_vec3_to_json(const glm::vec3 &vec)
{
    return {{"x", vec.x}, {"y", vec.y}, {"z", vec.z}};
}

std::string util::get_json_relative_path(const std::filesystem::path &full_path)
{
    const std::string folder = full_path.parent_path().filename().string();

    const std::string file = full_path.filename().string();

    return folder + "/" + file;
}
} // namespace sdk