#include "entity_component/EntityManager.hpp"

Entity EntityManager::createEntity()
{
    /// TODO: Keep track of Ids?
    return idGenerator.getNewId();
}

void EntityManager::destroyEntity(Entity entity)
{
    for (auto &componentStore : componentStores)
    {
        componentStore->destroyComponent(entity);
    }
}