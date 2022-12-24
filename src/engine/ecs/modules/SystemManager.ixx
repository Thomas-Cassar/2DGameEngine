module;
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>
export module Ecs:SystemManager;

import :ISystem;
import Core;

namespace Ecs
{
export class SystemManager {
public:
    SystemManager(std::shared_ptr<EntityManager> manager);

    template <Derived<ISystem> T>
    void registerSystem()
    {
        auto const& system{getSystem<T>()};
        if (system)
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
    Core::IdGenerator systemIdGenerator;

    template <Derived<ISystem> T>
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

    template <Derived<ISystem> T>
    std::shared_ptr<T> getSystem()
    {
        size_t const hash{getSystemIdx<T>()};
        auto const& system{typeToSystem.find(hash)};
        return system != std::end(typeToSystem) ? std::dynamic_pointer_cast<T>(system->second) : nullptr;
    }
};
} // namespace Ecs
