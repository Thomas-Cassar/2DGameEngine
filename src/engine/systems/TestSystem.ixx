module;
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include "imgui.h"
#include <string>
export module System:TestSystem;

import Component;
import Ecs;

namespace System
{

export class TestSystem : public Ecs::ISystem {
public:
    void update(Ecs::EntityManager& manager, float deltaTime_s) override
    {
        ImGui::Begin("Debug Window");
        Ecs::ComponentsForEachFn<Component::DiffuseLightComponent, Component::TransformComponent> const forEachDiffuse{
            [](Ecs::Entity entity, Component::DiffuseLightComponent& diffLight,
               Component::TransformComponent& transform) {
                float lightPos[3] = {transform.position.x, transform.position.y,
                                     transform.position.z};
                ImGui::SliderFloat3("Light position",lightPos,-100.0f,100.0f);
                transform.position = {lightPos[0], lightPos[1], lightPos[2]};
                return true;
            }};

        manager.forEachComponents(forEachDiffuse);
        ImGui::End();
    }
};
} // namespace System
