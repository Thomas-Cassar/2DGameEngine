module;
#include "glm/glm.hpp"
export module Component:MovementComponent;

import Ecs;

namespace Component
{
export struct MovementComponent : public Ecs::IComponent {
    glm::vec3 velocity = {};
    float terminalVelocity = INFINITY;
    bool terminalVelocityAffectsVertical = false;
    glm::vec3 acceleration = {};
    glm::vec3 angularVelocity = {};
    glm::vec3 angularAcceleration = {};
    bool doesGravity = false;
    bool canMove = true;

    MovementComponent() {}

    MovementComponent(bool doesGravity, bool canMove) : doesGravity(doesGravity), canMove(canMove) {}

    MovementComponent(bool doesGravity, bool canMove, float terminalVelocity)
        : doesGravity(doesGravity), canMove(canMove), terminalVelocity(terminalVelocity)
    {
    }
};
} // namespace Component
