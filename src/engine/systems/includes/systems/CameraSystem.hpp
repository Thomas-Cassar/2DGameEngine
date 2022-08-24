#pragma once

#include "components/CameraComponent.hpp"
#include "components/TransformComponent.hpp"
#include "ecs/ISystem.hpp"

class CameraSystem : public ISystem {
public:
    void update(EntityManager& manager, float deltaTime_s) override;

    static void moveForward(CameraComponent const& camera, TransformComponent& transform, float distance);

    static void moveRight(CameraComponent const& camera, TransformComponent& transform, float distance);

    static void turnCameraFromInput(CameraComponent& camera, TransformComponent& transform,
                                    glm::dvec2 const& deltaInput);

    static void updateCameraProjectionMatrix(CameraComponent& camera, float const fov, float const width,
                                             float const height);
};