#include "ecs/SystemManager.hpp"

SystemManager::SystemManager(std::shared_ptr<EntityManager> manager) : m_Manager(manager) {}

void SystemManager::updateSystems(float deltaTime_s)
{
    for (auto const& system : systems)
    {
        system->update(*m_Manager, deltaTime_s);
    }
}