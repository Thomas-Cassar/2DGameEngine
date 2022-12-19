module;
#include "glm/glm.hpp"
export module Component:AmbientLightComponent;

import Ecs;

namespace Component
{
export struct AmbientLightComponent : public Ecs::IComponent {
    float strength = 0.4f;
    glm::vec3 color = {1.0f, 1.0f, 1.0f};

    AmbientLightComponent() {}
    AmbientLightComponent(float strength, glm::vec3 color) : strength(strength), color(color) {}
};
}
