#pragma once
#include "ecs/ISystem.hpp"

#include <iostream>

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
        typeToSystem.emplace(getSystemIdx<T>(), newSystem);
    }

    void updateSystems(float deltaTime_s);

private:
    std::vector<std::shared_ptr<ISystem>> systems;
    std::unordered_map<size_t, std::shared_ptr<ISystem>> typeToSystem;
    std::shared_ptr<EntityManager> m_Manager;
    IdGenerator systemIdGenerator;

    template <typename T>
    size_t getSystemIdx()
    {
        static_assert(std::is_base_of<ISystem, T>::value, "Systems must be derived from ISystem");
        static size_t idx = 0;
        static bool idGenerated = false;

        if (!idGenerated)
        {
            idx = systemIdGenerator.getNewId();
            idGenerated = true;
        }

        return idx;
    }

    template <typename T>
    std::shared_ptr<T> getSystem()
    {
        size_t const hash{getSystemIdx<T>()};
        auto const& system{typeToSystem.find(hash)};
        return system != std::end(typeToSystem) ? std::dynamic_pointer_cast<T>(system->second) : nullptr;
    }
};