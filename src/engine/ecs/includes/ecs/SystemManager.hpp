#pragma once
#include "ecs/ISystem.hpp"

class SystemManager {
public:
    SystemManager(std::shared_ptr<EntityManager> manager);

    template <typename T>
    void registerSystem()
    {
        static_assert(std::is_base_of<ISystem, T>::value, "Systems must be derived from ISystem");

        auto const& system{getSystem<T>()};
        if (system != nullptr)
        {
            std::cerr << "System already registered" << std::endl;
            return;
        }

        std::shared_ptr<T> newSystem{std::make_shared<T>()};
        systems.emplace_back(newSystem);
        typeToSystem.emplace(typeid(T).hash_code(), newSystem);
    }

    void updateSystems();

private:
    std::vector<std::shared_ptr<ISystem>> systems;
    std::unordered_map<size_t, std::shared_ptr<ISystem>> typeToSystem;
    std::shared_ptr<EntityManager> m_Manager;

    template <typename T>
    std::shared_ptr<T> getSystem()
    {
        size_t const hash{typeid(T).hash_code()};
        auto const& system{typeToSystem.find(hash)};
        return system != std::end(typeToSystem) ? std::dynamic_pointer_cast<T>(system->second) : nullptr;
    }
};