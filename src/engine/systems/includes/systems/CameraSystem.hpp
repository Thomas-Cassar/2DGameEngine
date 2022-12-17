#pragma once

#include "ecs/ISystem.hpp"

import Component;

class CameraSystem : public ISystem {
public:
    void update(EntityManager& manager, float deltaTime_s) override;

    static void moveForward(Component::CameraComponent const& camera, Component::TransformComponent& transform,
                            float distance);

    static void moveRight(Component::CameraComponent const& camera, Component::TransformComponent& transform,
                          float distance);

    static void turnCameraFromInput(Component::CameraComponent& camera, Component::TransformComponent& transform,
                                    glm::dvec2 const& deltaInput);

    static void updateCameraProjectionMatrix(Component::CameraComponent& camera, float const fov, float const width,
                                             float const height);
};