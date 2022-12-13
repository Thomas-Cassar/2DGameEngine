#pragma once
#include "components/IComponent.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

struct TransformComponent : public IComponent {
    glm::vec3 position = {};
    glm::quat rotation = glm::quat(glm::vec3{});
    glm::vec3 scale = {1.0f, 1.0f, 1.0f};

    TransformComponent(glm::vec3 position, glm::quat quaternion) : position(position), rotation(quaternion) {}

    TransformComponent(glm::vec3 position, glm::vec3 rotation) : position(position), rotation(glm::quat(rotation)) {}

    TransformComponent(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
        : position(position), rotation(glm::quat(rotation)), scale(scale)
    {
    }

    TransformComponent(glm::vec3 position) : position(position) {}

    TransformComponent() {}
};