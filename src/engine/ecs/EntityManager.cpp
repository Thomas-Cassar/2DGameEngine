#include "ecs/EntityManager.hpp"

EntityManager::EntityManager() noexcept { componentStores.reserve(RESERVED_COMPONENT_STORES); }

Entity EntityManager::createEntity()
{
    /// TODO: Keep track of Ids?
    return idGenerator.getNewId();
}

void EntityManager::destroyEntity(Entity entity)
{
    for (auto& componentStore : componentStores)
    {
        componentStore->destroyComponent(entity);
    }
}