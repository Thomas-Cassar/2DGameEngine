#pragma once
#include "entity_component/ComponentStore.hpp"
#include "entity_component/Entity.hpp"
#include "entity_component/IdGenerator.hpp"
#include <memory>
#include <unordered_map>
#include <vector>

class EntityManager {
public:
    Entity createEntity();

    void destroyEntity(Entity entity);

    template <typename T> void registerComponent()
    {
        auto const &componentStore{getComponentStore<T>()};
        if (componentStore != nullptr)
        {
            std::cerr << "Component already registered" std::endl;
            return;
        }

        std::shared_ptr<ComponentStore<T>> newComponentStore;
        componentStores.push_back(newComponentStore);

        size_t componentType{typeid(T).hash_code};
        typeToComponentStore.emplace(componentType, newComponentStore);
    }

    template <typename T> void addComponent(Entity entity, T &&component)
    {
        std::shared_ptr<ComponentStore<T>> componentStore{getComponentStore<T>()};
        if (componentStore == nullptr)
        {
            throw Exception("Tried to add a component without registering!");
        }
        componentStore->addComponent(entity, component);
    }

    template <typename T> void forEachComponent(ComponentForEachFn<T> fn)
    {
        std::shared_ptr<ComponentStore<T>> componentStore{getComponentStore<T>()};
        if (componentStore == nullptr)
        {
            throw Exception("Could not find component to do for each!");
        }
        componentStore->forEach(fn);
    }

private:
    std::vector<std::shared_ptr<IComponentStore>> componentStores;
    std::unordered_map<size_t, std::shared_ptr<IComponentStore>> typeToComponentStore;
    IdGenerator idGenerator;

    template <typename T> std::shared_ptr<ComponentStore<T>> getComponentStore()
    {
        size_t componentType{typeid(T).hash_code};
        auto const &componentStore{
            std::find_if(std::begin(typeToComponentStore), std::end(typeToComponentStore), [](auto const &it) {
                return it->first() == componentType &&
                       std::dynamic_pointer_cast<ComponentStore<T>>(it->second) != nullptr;
            })};
        return componentStore != std::end(typeToComponentStore)
                   ? std::dynamic_pointer_cast<ComponentStore<T>>(componentStore->second)
                   : std::make_shared(nullptr);
    }
};