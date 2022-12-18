export module Ecs:EntityManager;
import <concepts>;
import <exception>;
import <iostream>;
import <memory>;
import <sstream>;
import <tuple>;
import <unordered_map>;
import <vector>;

import Core;
import :ComponentStore;
import :Entity;
import :IComponent;

namespace Ecs
{
constexpr size_t kReservedComponentStores = 128;

export template <Derived<IComponent> T, Derived<IComponent>... Args>
using ComponentsForEachFn = std::function<bool(Entity, T&, Args&...)>;

export class EntityManager {
public:
    EntityManager() noexcept { componentStores.reserve(kReservedComponentStores); }

    Entity createEntity() { return entityIdGenerator.getNewId(); }

    void destroyEntity(Entity entity)
    {
        for (auto& componentStore : componentStores)
        {
            componentStore->destroyComponent(entity);
        }
    }

    template <Derived<IComponent> T>
    void registerComponent()
    {
        std::shared_ptr<ComponentStore<T>> componentStore{getComponentStore<T>()};
        if (componentStore)
        {
            std::cerr << "Component already registered" << std::endl;
            return;
        }

        std::shared_ptr<ComponentStore<T>> newComponentStore{std::make_shared<ComponentStore<T>>()};
        componentStores.emplace_back(newComponentStore);
        typeToComponentStore.emplace(getComponentIdx<T>(), newComponentStore);
    }

    template <Derived<IComponent> T>
    void addComponent(Entity entity, T&& component)
    {
        std::shared_ptr<ComponentStore<T>> componentStore{getComponentStore<T>()};
        if (!componentStore)
        {
            throw std::exception("Tried to add a component without registering!");
        }
        componentStore->addComponent(entity, std::move(component));
    }

    template <Derived<IComponent> T>
    void addComponent(Entity entity, T const& component)
    {
        std::shared_ptr<ComponentStore<T>> componentStore{getComponentStore<T>()};
        if (!componentStore)
        {
            throw std::exception("Tried to add a component without registering!");
        }
        componentStore->addComponent(entity, component);
    }

    template <Derived<IComponent> T, Derived<IComponent>... Args>
    bool forEachComponents(ComponentsForEachFn<T, Args...> const& forEachFn)
    {
        std::shared_ptr<ComponentStore<T>> componentStore{getComponentStore<T>()};
        if (!componentStore)
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

    template <Derived<IComponent>... Args, typename = std::enable_if<(sizeof...(Args) == 0), size_t>::type>
    bool hasComponents(Entity entity)
    {
        return true;
    }

    template <Derived<IComponent> T, Derived<IComponent>... Args>
    bool hasComponents(Entity entity)
    {
        std::shared_ptr<ComponentStore<T>> componentStore{getComponentStore<T>()};
        if (!componentStore)
        {
            return false;
        }
        return componentStore->hasComponent(entity) && hasComponents<Args...>(entity);
    }

    template <Derived<IComponent> T>
    T& getComponent(Entity entity)
    {
        std::shared_ptr<ComponentStore<T>> componentStore{getComponentStore<T>()};
        if (!componentStore)
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
    Core::IdGenerator entityIdGenerator;
    Core::IdGenerator componentIdGenerator;

    template <Derived<IComponent> T>
    size_t getComponentIdx()
    {
        static size_t idx = 0;
        static bool idGenerated = false;

        if (!idGenerated)
        {
            idx = componentIdGenerator.getNewId();
            idGenerated = true;
        }

        return idx;
    }

    template <Derived<IComponent> T>
    std::shared_ptr<ComponentStore<T>> getComponentStore()
    {
        size_t const hash{getComponentIdx<T>()};
        auto const& componentStore{typeToComponentStore.find(hash)};
        return componentStore != std::end(typeToComponentStore)
                   ? std::dynamic_pointer_cast<ComponentStore<T>>(componentStore->second)
                   : nullptr;
    }

    template <Derived<IComponent> T>
    T* getComponentPtr(Entity entity)
    {
        std::shared_ptr<ComponentStore<T>> componentStore{getComponentStore<T>()};
        if (!componentStore)
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
        else if (!std::get<I>(tup))
        {
            return true;
        }
        else
        {
            return isTupleNull<I + 1, Args...>(tup);
        }
    }
};
} // namespace Ecs