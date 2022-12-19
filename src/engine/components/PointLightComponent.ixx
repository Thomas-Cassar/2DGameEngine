module;
#include "glm/glm.hpp"
export module Component:PointLightComponent;

import Ecs;

namespace Component
{
export struct PointLightComponent : public Ecs::IComponent {
    float diffuseStrength = 1.0f;
    float specularStength = 0.5f;
    glm::vec3 color = {1.0f, 1.0f, 1.0f};

    PointLightComponent() {}

    PointLightComponent(float diffuseStrength, glm::vec3 color) : diffuseStrength(diffuseStrength), color(color)
    {
    }

};
}
