module Ecs:EntityManager;

namespace Ecs
{
EntityManager::EntityManager() noexcept { componentStores.reserve(kReservedComponentStores); }

Entity EntityManager::createEntity() { return entityIdGenerator.getNewId(); }

void EntityManager::destroyEntity(Entity entity)
{
    for (auto& componentStore : componentStores)
    {
        componentStore->destroyComponent(entity);
    }
}
}
