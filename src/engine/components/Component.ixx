module;
#include <compare>
export module Component;

export import :BoxCollision;
export import :CameraComponent;
export import :InputComponent;
export import :ModelComponent;
export import :MovementComponent;
export import :PlayerComponent;
export import :TransformComponent;
export import :AmbientLightComponent;
export import :PointLightComponent;
import Ecs;

namespace Component
{
export void registerComponents(Ecs::EntityManager& manager)
{
    manager.registerComponent<TransformComponent>();
    manager.registerComponent<ModelComponent>();
    manager.registerComponent<CameraComponent>();
    manager.registerComponent<InputComponent>();
    manager.registerComponent<MovementComponent>();
    manager.registerComponent<BoxCollision>();
    manager.registerComponent<PlayerComponent>();
    manager.registerComponent<AmbientLightComponent>();
    manager.registerComponent<PointLightComponent>();
}
} // namespace Component