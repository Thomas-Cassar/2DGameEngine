#pragma once
#include "components/IComponent.hpp"
#include "glm/glm.hpp"

struct MovementComponent : public IComponent {
    glm::vec3 velocity = {};
    glm::vec3 acceleration = {};
    glm::vec3 angularVelocity = {};
    glm::vec3 angularAcceleration = {};
    bool doesGravity = false;

    MovementComponent() {}

    MovementComponent(bool doesGravity) : doesGravity(doesGravity) {}
};