#include "ecs/EntityManager.hpp"

EntityManager::EntityManager() noexcept { componentStores.reserve(RESERVED_COMPONENT_STORES); }

Entity EntityManager::createEntity() { return entityIdGenerator.getNewId(); }

void EntityManager::destroyEntity(Entity entity)
{
    for (auto& componentStore : componentStores)
    {
        componentStore->destroyComponent(entity);
    }
}