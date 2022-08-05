#pragma once

#include "components/CameraComponent.hpp"
#include "components/TranslationComponent.hpp"
#include "ecs/ISystem.hpp"

class CameraSystem : public ISystem {
public:
    void update(EntityManager& manager) override;

    static void moveForward(CameraComponent const& camera, TranslationComponent& translation, float distance);

    static void moveRight(CameraComponent const& camera, TranslationComponent& translation, float distance);

    static void turnCameraFromInput(CameraComponent& camera, TranslationComponent& translation,
                                    glm::dvec2 const& deltaInput);

    static void updateCameraProjectionMatrix(CameraComponent& camera, float const fov, float const width,
                                             float const height);
};