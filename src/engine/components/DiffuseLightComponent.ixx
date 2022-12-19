module;
#include "glm/glm.hpp"
export module Component:DiffuseLightComponent;

import Ecs;

namespace Component
{
export struct DiffuseLightComponent : public Ecs::IComponent {
    float strength = 2.0f;
    glm::vec3 color = {1.0f, 1.0f, 1.0f};

    DiffuseLightComponent() {}

    DiffuseLightComponent(float strength, glm::vec3 color, glm::vec3 direction)
        : strength(strength), color(color)
    {
    }

};
}
