#pragma once

#include <filesystem>

#include <glm/glm.hpp>

#include <iostream>
#include <nlohmann/json.hpp>

namespace sdk::suppl
{
glm::vec3 glm_vec3_from_json(const nlohmann::json &j,
                             const glm::vec3 &default_value);
nlohmann::json glm_vec3_to_json(const glm::vec3 &vec);
} // namespace sdk::suppl

namespace sdk
{

struct transform
{
    glm::vec3 _position{0.0f, 0.0f, 0.0f};
    glm::vec3 _scale{1.0f, 1.0f, 1.0f};
    float _current_rotation_angle = 0.0f;

    [[nodiscard]] static transform deserialize(const nlohmann::json &input_json)
    {

        return {
            ._position = suppl::glm_vec3_from_json(
                input_json.value("position", nlohmann::json::object()),
                {0.0f, 0.0f, 0.0f}),
            ._scale = suppl::glm_vec3_from_json(
                input_json.value("scale", nlohmann::json::object()),
                {1.0f, 1.0f, 1.0f}),

            ._current_rotation_angle = input_json.value("rotation_angle", 0.0f),
        };
    }

    [[nodiscard]] nlohmann::json serialize() const
    {
        nlohmann::json result{
            {"position", nlohmann::json::object()},
            {"scale", nlohmann::json::object()},
            {"rotation_angle", _current_rotation_angle},
        };
        result.at("position").merge_patch(suppl::glm_vec3_to_json(_position));
        result.at("scale").merge_patch(suppl::glm_vec3_to_json(_scale));
        return result;
    }
};
} // namespace sdk
