module;
#include "glm/glm.hpp"
export module Component:ColorComponent;

import :IComponent;

namespace Component
{
export struct ColorComponent : public IComponent {
    glm::vec4 color = {1.0F, 1.0F, 1.0F, 1.0F};

    ColorComponent() {}

    ColorComponent(glm::vec4 color) : color(color) {}
};
} // namespace Component