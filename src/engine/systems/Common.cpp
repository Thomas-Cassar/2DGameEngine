#include "systems/Common.hpp"
#include "systems/MeshSystem.hpp"

#include "systems/CameraSystem.hpp"
#include "systems/CollisionSystem.hpp"
#include "systems/InputSystem.hpp"
#include "systems/MovementSystem.hpp"
#include "systems/PlayerSystem.hpp"
#include "systems/TestSystem.hpp"

void registerSystems(SystemManager& manager)
{
    // Order of registration is order of update call
    manager.registerSystem<InputSystem>();
    manager.registerSystem<CameraSystem>();
    manager.registerSystem<PlayerSystem>();
    manager.registerSystem<MovementSystem>();
    manager.registerSystem<CollisionSystem>();
    manager.registerSystem<MeshSystem>();
}