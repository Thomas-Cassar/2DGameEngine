#pragma once
#include "components/IComponent.hpp"
#include "glm/glm.hpp"

struct MovementComponent : public IComponent {
    glm::vec3 velocity = {};
    glm::vec3 acceleration = {};
    bool doesGravity = false;

    MovementComponent() {}

    MovementComponent(bool doesGravity) : doesGravity(doesGravity) {}
};