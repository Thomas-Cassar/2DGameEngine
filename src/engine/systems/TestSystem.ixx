module;
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include "imgui.h"
export module System:TestSystem;

import <string>;
import Component;
import Ecs;

namespace System
{

export class TestSystem : public Ecs::ISystem {
public:
    void update(Ecs::EntityManager& manager, float deltaTime_s) override
    {
        ImGui::Begin("Debug Window");
        int i{};
        Ecs::ComponentsForEachFn<Component::TransformComponent, Component::MeshComponent,
                                 Component::MovementComponent> const forEachTransform{
            [&manager, &i](Ecs::Entity entity, Component::TransformComponent& transform, Component::MeshComponent& mesh,
                           Component::MovementComponent& move) {
                float pos[3]{transform.position.x, transform.position.y, transform.position.z};
                ImGui::SliderFloat3(std::string("Cube Position " + std::to_string(i)).c_str(), pos, -15.0F, 15.0F);
                transform.position.x = pos[0];
                transform.position.y = pos[1];
                transform.position.z = pos[2];
                glm::vec3 eulerRot_deg{glm::degrees(glm::eulerAngles(transform.rotation))};
                float rot[3]{eulerRot_deg.x, eulerRot_deg.y, eulerRot_deg.z};
                ImGui::SliderFloat3(std::string("Cube Rotation " + std::to_string(i)).c_str(), rot, -180.0F, 180.0F);
                eulerRot_deg.x = rot[0];
                eulerRot_deg.y = rot[1];
                eulerRot_deg.z = rot[2];
                transform.rotation = glm::quat(glm::radians(eulerRot_deg));

                float angAcc[3]{move.angularAcceleration.x, move.angularAcceleration.y, move.angularAcceleration.z};
                ImGui::InputFloat3(std::string("Cube Angular Acceleration " + std::to_string(i)).c_str(), angAcc);
                move.angularAcceleration.x = angAcc[0];
                move.angularAcceleration.y = angAcc[1];
                move.angularAcceleration.z = angAcc[2];
                i++;
                return true;
            }};

        manager.forEachComponents(forEachTransform);
        ImGui::End();
    }
};
} // namespace System
