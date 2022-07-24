#pragma once
#include "entity_component/Entity.hpp"
#include <vector>
#include <memory>
#include <utility>

class EntityManager
{
public:
    template<typename T, typename... Args>
    std::shared_ptr<T> registerEntity(Args&&... args)
    {
        static_assert(std::is_base_of<Entity,T>::value,"T must be derived from entity base class");

        std::shared_ptr<T> newEntity{std::make_shared<T>(std::forward<Args>(args)...)};
        aliveEntities.push_back(newEntity);
        return newEntity;
    }
    void removeEntity(std::shared_ptr<Entity> const& entity);

private:
    std::vector<std::shared_ptr<Entity>> aliveEntities;
};