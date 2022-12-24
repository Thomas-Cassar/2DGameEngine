module;
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include "imgui.h"
#include <string>
module System:TestSystem;

import Component;
import Ecs;

namespace System
{

void TestSystem::update(Ecs::EntityManager& manager, float deltaTime_s)
{
    ImGui::Begin("Debug Window");
    Ecs::ComponentsForEachFn<Component::PointLightComponent, Component::TransformComponent> const forEachDiffuse{
        [](Ecs::Entity entity, Component::PointLightComponent& light, Component::TransformComponent& transform) {
            float lightPos[3] = {transform.position.x, transform.position.y, transform.position.z};
            ImGui::SliderFloat3("Light position", lightPos, -100.0f, 100.0f);
            transform.position = {lightPos[0], lightPos[1], lightPos[2]};

            ImGui::InputFloat("Specular Intensity", &light.specularStength);
            ImGui::InputFloat("Diffuse Intensity", &light.diffuseStrength);

            float lightColor[3] = {light.color.x, light.color.y, light.color.z};
            ImGui::ColorPicker3("Light Color", lightColor);
            light.color = {lightColor[0], lightColor[1], lightColor[2]};

            return true;
        }};

    manager.forEachComponents(forEachDiffuse);
    ImGui::End();
}

} // namespace System
