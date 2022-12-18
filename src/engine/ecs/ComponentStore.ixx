export module Ecs:ComponentStore;

import <exception>;
import <functional>;
import <unordered_map>;
import <vector>;
import :Entity;
import :IComponentStore;
import :IComponent;
import Core;

namespace Ecs
{
export template <Derived<IComponent> T>
using ComponentForEachFn = std::function<bool(Entity, T&)>;

export template <Derived<IComponent> T>
class ComponentStore : public Ecs::IComponentStore {
public:
    void addComponent(Entity entity, T&& component)
    {
        components.emplace_back(std::move(component));
        componentToEntity.emplace_back(entity);
        entityToComponent.emplace(entity, components.size() - 1);
    }

    void addComponent(Entity entity, T const& component)
    {
        components.emplace_back(component);
        componentToEntity.emplace_back(entity);
        entityToComponent.emplace(entity, components.size() - 1);
    }

    void destroyComponent(Entity entity) override
    {
        auto component{entityToComponent.find(entity)};
        if (component == std::end(entityToComponent))
        {
            return;
        }
        size_t index{component->second};
        if (index != components.size() - 1)
        {
            // Move last component to index of component we are removing so indicies do not change
            components.at(index) = std::move(components.at(components.size() - 1));
            componentToEntity.at(index) = componentToEntity.at(componentToEntity.size() - 1);
        }
        // Remove component
        components.erase(std::end(components) - 1);
        componentToEntity.erase(std::end(componentToEntity) - 1);
        entityToComponent.erase(component);
    }

    T* getComponentPtr(Entity const entity)
    {
        auto component{entityToComponent.find(entity)};
        if (component == std::end(entityToComponent))
        {
            return nullptr;
        }
        return &components.at(component->second);
    }

    T& getComponent(Entity const entity)
    {
        auto component{entityToComponent.find(entity)};
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
    std::vector<Entity> componentToEntity;
    std::unordered_map<Entity, size_t> entityToComponent;
};
} // namespace Ecs