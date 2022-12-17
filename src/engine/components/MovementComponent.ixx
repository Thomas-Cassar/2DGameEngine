module;
#include "glm/glm.hpp"
export module Component:MovementComponent;

import :IComponent;

namespace Component
{
export struct MovementComponent : public IComponent {
    glm::vec3 velocity = {};
    float terminalVelocityVertical = INFINITY;
    float terminalVelocityHorizontal = INFINITY;
    glm::vec3 acceleration = {};
    glm::vec3 angularVelocity = {};
    glm::vec3 angularAcceleration = {};
    bool doesGravity = false;
    bool canMove = true;

    MovementComponent() {}

    MovementComponent(bool doesGravity, bool canMove) : doesGravity(doesGravity), canMove(canMove) {}

    MovementComponent(bool doesGravity, bool canMove, float terminalVelocityHorizontal)
        : doesGravity(doesGravity), canMove(canMove), terminalVelocityHorizontal(terminalVelocityHorizontal)
    {
    }
};
} // namespace Component
