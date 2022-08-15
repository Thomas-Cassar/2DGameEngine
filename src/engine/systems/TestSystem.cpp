#include "systems/TestSystem.hpp"
#include "components/MeshComponent.hpp"
#include "components/MovementComponent.hpp"
#include "imgui.h"

void TestSystem::update(EntityManager& manager, float deltaTime_s)
{
    ImGui::Begin("Debug Window");
    int i{};
    ComponentsForEachFn<TranslationComponent, MeshComponent, MovementComponent> const forEachTranslation{
        [&manager, &i](Entity entity, TranslationComponent& translation, MeshComponent& mesh, MovementComponent& move) {
            float pos[3]{translation.position.x, translation.position.y, translation.position.z};
            ImGui::SliderFloat3(std::string("Cube Position " + std::to_string(i)).c_str(), pos, -15.0F, 15.0F);
            translation.position.x = pos[0];
            translation.position.y = pos[1];
            translation.position.z = pos[2];
            glm::vec3 eulerRot_deg{glm::degrees(glm::eulerAngles(translation.rotation))};
            float rot[3]{eulerRot_deg.x, eulerRot_deg.y, eulerRot_deg.z};
            ImGui::SliderFloat3(std::string("Cube Rotation " + std::to_string(i)).c_str(), rot, -180.0F, 180.0F);
            eulerRot_deg.x = rot[0];
            eulerRot_deg.y = rot[1];
            eulerRot_deg.z = rot[2];
            translation.rotation = glm::quat(glm::radians(eulerRot_deg));

            float angAcc[3]{move.angularAcceleration.x, move.angularAcceleration.y, move.angularAcceleration.z};
            ImGui::InputFloat3(std::string("Cube Angular Acceleration " + std::to_string(i)).c_str(), angAcc);
            move.angularAcceleration.x = angAcc[0];
            move.angularAcceleration.y = angAcc[1];
            move.angularAcceleration.z = angAcc[2];
            i++;
            return true;
        }};

    manager.forEachComponents<TranslationComponent, MeshComponent, MovementComponent>(forEachTranslation);
    ImGui::End();
}
