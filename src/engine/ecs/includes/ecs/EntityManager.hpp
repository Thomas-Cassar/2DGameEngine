#pragma once
#include "ecs/ComponentStore.hpp"
#include "ecs/Entity.hpp"
#include "ecs/IdGenerator.hpp"
#include <exception>
#include <iostream>
#include <memory>
#include <sstream>
#include <tuple>
#include <unordered_map>
#include <vector>

#define RESERVED_COMPONENT_STORES 128

template <typename T, typename... Args>
using ComponentsForEachFn = std::function<bool(Entity, T&, Args&...)>;

struct IComponent;

class EntityManager {
public:
    EntityManager() noexcept;

    Entity createEntity();

    void destroyEntity(Entity entity);

    template <typename T>
    void registerComponent()
    {
        static_assert(std::is_base_of<IComponent, T>::value, "Components must be derived from IComponent");
        auto const& componentStore{getComponentStore<T>()};
        if (componentStore != nullptr)
        {
            std::cerr << "Component already registered" << std::endl;
            return;
        }

        std::shared_ptr<ComponentStore<T>> newComponentStore{std::make_shared<ComponentStore<T>>()};
        componentStores.emplace_back(newComponentStore);
        typeToComponentStore.emplace(typeid(T).hash_code(), newComponentStore);
    }

    template <typename T>
    void addComponent(Entity entity, T&& component)
    {
        std::shared_ptr<ComponentStore<T>> componentStore{getComponentStore<T>()};
        if (componentStore == nullptr)
        {
            throw std::exception("Tried to add a component without registering!");
        }
        componentStore->addComponent(entity, std::move(component));
    }

    template <typename T>
    void addComponent(Entity entity, T const& component)
    {
        std::shared_ptr<ComponentStore<T>> componentStore{getComponentStore<T>()};
        if (componentStore == nullptr)
        {
            throw std::exception("Tried to add a component without registering!");
        }
        componentStore->addComponent(entity, component);
    }

    template <typename T, typename... Args>
    bool forEachComponents(ComponentsForEachFn<T, Args...> const& forEachFn)
    {
        std::shared_ptr<ComponentStore<T>> componentStore{getComponentStore<T>()};
        if (componentStore == nullptr)
        {
            throw std::exception("Could not find component to do for each!");
        }

        ComponentForEachFn<T> const checkedForEachFn{[this, &forEachFn](Entity entity, T& component) {
            std::tuple<Args*...> componentPack = std::tuple<Args*...>(getComponentPtr<Args>(entity)...);
            if (isTupleNull(componentPack))
            {
                return true;
            }

            auto const ptrTupleToRef{
                [](Args*... arg) -> std::tuple<Args&...> { return std::tie<Args&...>((*arg)...); }};
            std::tuple<Args&...> componentRefPack = std::apply(ptrTupleToRef, componentPack);

            return std::apply(forEachFn, std::tuple_cat(std::tie(entity, component), std::move(componentRefPack)));
        }};

        return componentStore->forEach(checkedForEachFn);
    }

    template <typename... Args, typename = std::enable_if<(sizeof...(Args) == 0), size_t>::type>
    bool hasComponents(Entity entity)
    {
        return true;
    }

    template <typename T, typename... Args>
    bool hasComponents(Entity entity)
    {
        std::shared_ptr<ComponentStore<T>> componentStore{getComponentStore<T>()};
        if (componentStore == nullptr)
        {
            return false;
        }
        return componentStore->hasComponent(entity) && hasComponents<Args...>(entity);
    }

    template <typename T>
    T& getComponent(Entity entity)
    {
        std::shared_ptr<ComponentStore<T>> componentStore{getComponentStore<T>()};
        if (componentStore == nullptr)
        {
            std::stringstream message;
            message << "Could not retrieve component " << typeid(T).name() << " for entity with ID " << entity;
            throw std::exception(message.str().c_str());
        }
        return componentStore->getComponent(entity);
    }

private:
    std::vector<std::shared_ptr<IComponentStore>> componentStores;
    std::unordered_map<size_t, std::shared_ptr<IComponentStore>> typeToComponentStore;
    IdGenerator idGenerator;

    template <typename T>
    std::shared_ptr<ComponentStore<T>> getComponentStore()
    {
        size_t const hash{typeid(T).hash_code()};
        auto const& componentStore{typeToComponentStore.find(hash)};
        return componentStore != std::end(typeToComponentStore)
                   ? std::dynamic_pointer_cast<ComponentStore<T>>(componentStore->second)
                   : nullptr;
    }

    template <typename T>
    T* getComponentPtr(Entity entity)
    {
        std::shared_ptr<ComponentStore<T>> componentStore{getComponentStore<T>()};
        if (componentStore == nullptr)
        {
            std::stringstream message;
            message << "Could not retrieve component " << typeid(T).name() << " for entity with ID " << entity;
            throw std::exception(message.str().c_str());
        }
        return componentStore->getComponentPtr(entity);
    }

    template <size_t I = 0, typename... Args>
    bool isTupleNull(std::tuple<Args*...> const& tup)
    {
        if constexpr (I == sizeof...(Args))
        {
            return false;
        }
        else if (std::get<I>(tup) == nullptr)
        {
            return true;
        }
        else
        {
            return isTupleNull<I + 1, Args...>(tup);
        }
    }
};