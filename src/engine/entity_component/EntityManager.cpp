#include "entity_component/EntityManager.hpp"

void EntityManager::removeEntity(std::shared_ptr<Entity> const& entity)
{
    for(auto& it{std::begin(aliveEntities)};it!=std::end(aliveEntities);++it)
    {
        if(entity==*it)
            aliveEntities.erase(it);
    }
}