#include "ecs/SystemManager.hpp"
#include "imgui.h"
#include <chrono>
#include <string>

SystemManager::SystemManager(std::shared_ptr<EntityManager> manager) : m_Manager(manager) {}

void SystemManager::updateSystems(float deltaTime_s)
{
    ImGui::Begin("Systems window");
    for (auto const& system : systems)
    {
        std::chrono::time_point start{std::chrono::steady_clock::now()};
        system->update(*m_Manager, deltaTime_s);
        ImGui::Text((std::string(typeid(system).raw_name()) + std::string(" took ") +
                     std::to_string(static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(
                                                           std::chrono::steady_clock::now() - start)
                                                           .count()) /
                                    1000.0F) +
                     std::string(" ms"))
                        .c_str());
    }
    ImGui::End();
}