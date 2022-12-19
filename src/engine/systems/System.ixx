export module System;

export import :CameraSystem;
export import :CollisionSystem;
export import :InputSystem;
export import :MeshSystem;
export import :MovementSystem;
export import :PlayerSystem;
export import :TestSystem;
import Ecs;

namespace System
{

export void registerSystems(Ecs::SystemManager& manager)
{
    // Order of registration is order of update call
    manager.registerSystem<InputSystem>();
    manager.registerSystem<PlayerSystem>();
    manager.registerSystem<CameraSystem>();
    manager.registerSystem<MovementSystem>();
    manager.registerSystem<CollisionSystem>();
    manager.registerSystem<MeshSystem>();
    manager.registerSystem<TestSystem>();
}
} // namespace System
