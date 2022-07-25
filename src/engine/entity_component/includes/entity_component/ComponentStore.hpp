#pragma once
#include "entity_component/Entity.hpp"
#include "entity_component/IComponentStore.hpp"
#include <functional>
#include <unordered_map>
#include <vector>

template <typename T> using ComponentForEachFn = std::function<void(T &)>;

template <typename T> class ComponentStore : public IComponentStore {
public:
    ComponentStore() {}

    void addComponent(Entity entity, T &&component)
    {
        components.emplace_back(component);
        entityToComponent.emplace(entity, components.size() - 1);
    }

    void destroyComponent(Entity entity) override
    {
        auto &component{entityToComponent.find(entity)};
        if (component == std::end(entityToComponent))
        {
            return;
        }
        size_t index{component->second};
        if (index != components.size() - 1)
        {
            // Swap last component and removed component so every other index stays the same
            std::swap(components.at(components.size() - 1), components.at(index));
        }
        // Remove component
        components.erase(components.size() - 1);
        entityToComponent.erase(component);
    }

    T &getComponent(Entity entity)
    {
        auto &component{entityToComponent.find(entity)};
        if (component == std::end(entityToComponent))
        {
            throw Exception("Could not find component!")
        }
        return component->second;
    }

    void forEach(ComponentForEachFn<T> fn)
    {
        for (T &component : components)
        {
            fn(component);
        }
    }

private:
    std::vector<T> components;
    std::unordered_map<Entity, size_t> entityToComponent;
};