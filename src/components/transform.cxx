#include "transform.hxx"

namespace sdk
{
glm::vec3 suppl::glm_vec3_from_json(const nlohmann::json &j,
                                    const glm::vec3 &default_value)
{
    return {
        j.value("x", default_value.x),
        j.value("y", default_value.y),
        j.value("z", default_value.z),
    };
}

nlohmann::json suppl::glm_vec3_to_json(const glm::vec3 &vec)
{
    return {{"x", vec.x}, {"y", vec.y}, {"z", vec.z}};
}
} // namespace sdk