#pragma once
#include "ecs/Entity.hpp"
#include "ecs/IComponentStore.hpp"
#include <exception>
#include <functional>
#include <unordered_map>
#include <vector>

template <typename T>
using ComponentForEachFn = std::function<bool(Entity entity, T&)>;

template <typename T>
class ComponentStore : public IComponentStore {
public:
    void addComponent(Entity entity, T&& component)
    {
        components.emplace_back(component);
        entityToComponent.emplace(entity, components.size() - 1);
        componentToEntity.emplace(components.size() - 1, entity);
    }

    void addComponent(Entity entity, T const& component)
    {
        components.emplace_back(component);
        entityToComponent.emplace(entity, components.size() - 1);
        componentToEntity.emplace(components.size() - 1, entity);
    }

    void destroyComponent(Entity entity) override
    {
        auto& component{entityToComponent.find(entity)};
        if (component == std::end(entityToComponent))
        {
            return;
        }
        size_t index{component->second};
        if (index != components.size() - 1)
        {
            // Move last component to index of component we are removing so indicies do not change
            components.at(index) = std::move(components.at(components.size() - 1));
        }
        // Remove component
        components.erase(std::end(components) - 1);
        entityToComponent.erase(component);
        componentToEntity.erase(componentToEntity.find(index));
    }

    T& getComponent(Entity const entity)
    {
        auto& component{entityToComponent.find(entity)};
        if (component == std::end(entityToComponent))
        {
            throw std::exception("Could not find component!");
        }
        return components.at(component->second);
    }

    bool hasComponent(Entity const entity)
    {
        auto const& component{entityToComponent.find(entity)};
        if (component == std::end(entityToComponent))
        {
            return false;
        }
        return true;
    }

    bool forEach(ComponentForEachFn<T> fn)
    {
        for (size_t i = 0; i < components.size(); i++)
        {
            if (!fn(componentToEntity[i], components[i]))
                return false;
        }
        return true;
    }

private:
    std::vector<T> components;
    std::unordered_map<Entity, size_t> entityToComponent;
    std::unordered_map<size_t, Entity> componentToEntity;
};