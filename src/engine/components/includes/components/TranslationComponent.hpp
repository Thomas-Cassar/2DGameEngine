#pragma once
#include "components/IComponent.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

struct TranslationComponent : public IComponent {
    glm::vec3 position = {};
    glm::quat rotation = {};

    TranslationComponent(glm::vec3 position, glm::quat quaternion) : position(position), rotation(quaternion) {}

    TranslationComponent(glm::vec3 position, glm::vec3 rotation) : position(position), rotation(glm::quat(rotation)) {}

    TranslationComponent(glm::vec3 position) : position(position) {}

    TranslationComponent() {}
};