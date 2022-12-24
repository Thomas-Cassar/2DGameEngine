module;
#include "glm/glm.hpp"
export module System:CameraSystem;
import Ecs;
import Component;

namespace System
{

export class CameraSystem : public Ecs::ISystem {
public:
    void update(Ecs::EntityManager& manager, float deltaTime_s) override;

    static void turnCameraFromInput(Component::CameraComponent& camera, Component::TransformComponent& transform,
                                    glm::dvec2 const& deltaInput);

    static void updateCameraProjectionMatrix(Component::CameraComponent& camera, float const fov, float const width,
                                             float const height);
};

} // namespace System
