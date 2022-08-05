#pragma once
#include "components/IComponent.hpp"
#include "glm/glm.hpp"

struct TranslationComponent : public IComponent {
    glm::vec3 position = {};
    glm::vec3 rotation = {};

    TranslationComponent(glm::vec3 position, glm::vec3 rotation) : position(position), rotation(rotation) {}

    TranslationComponent(glm::vec3 position) : position(position) {}

    TranslationComponent() {}
};