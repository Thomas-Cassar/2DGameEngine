#pragma once
#include "components/IComponent.hpp"
#include "glm/glm.hpp"

struct ColorComponent : public IComponent {
    glm::vec4 color = {1.0F, 1.0F, 1.0F, 1.0F};

    ColorComponent() {}

    ColorComponent(glm::vec4 color) : color(color) {}
};