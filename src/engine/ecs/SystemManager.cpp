#include "ecs/SystemManager.hpp"

SystemManager::SystemManager(std::shared_ptr<EntityManager> manager) : m_Manager(manager) {}

void SystemManager::updateSystems()
{
    for (auto const& system : systems)
    {
        system->update(*m_Manager);
    }
}