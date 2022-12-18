module;
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
export module System:MovementSystem;

import Component;
import Ecs;

namespace System
{
// Value to use for gravity for objects that experience gravity
constexpr float gravity{-20.0f};

export class MovementSystem : public Ecs::ISystem {
public:
    void update(Ecs::EntityManager& manager, float deltaTime_s) override
    {
        Ecs::ComponentsForEachFn<Component::MovementComponent, Component::TransformComponent> const forEachMovement{
            [&manager, &deltaTime_s](Ecs::Entity entity, Component::MovementComponent& movement,
                                     Component::TransformComponent& transform) {
                if (!movement.canMove)
                {
                    movement.acceleration = {};
                    movement.velocity = {};
                    movement.angularAcceleration = {};
                    movement.angularVelocity = {};
                    return true;
                }

                if (movement.doesGravity)
                {
                    movement.acceleration += glm::vec3{0.0F, gravity, 0.0F};
                }
                // Velocities for this time step is approximated to the new velocity
                movement.velocity += movement.acceleration * deltaTime_s;
                transform.position += movement.velocity * deltaTime_s;

                // Reset acceleration so forces can be applied again during the next frame
                movement.acceleration = {};

                // Clamp to terminal velocity
                glm::vec3 horizontalVelocity{movement.velocity.x, 0.0f, movement.velocity.z};
                float verticalVelocity{movement.velocity.y};
                if (glm::length(horizontalVelocity) > movement.terminalVelocityHorizontal)
                {
                    horizontalVelocity = glm::normalize(horizontalVelocity) * movement.terminalVelocityHorizontal;
                }
                if (std::fabs(verticalVelocity) > movement.terminalVelocityVertical)
                {
                    verticalVelocity = movement.terminalVelocityVertical;
                }
                movement.velocity = glm::vec3{horizontalVelocity.x, verticalVelocity, horizontalVelocity.z};

                // Angular
                // Derivation:
                // https://gamedev.stackexchange.com/questions/108920/applying-angular-velocity-to-quaternion
                transform.rotation =
                    (glm::quat(0.5F * movement.angularVelocity * deltaTime_s) + glm::quat()) * transform.rotation;
                movement.angularVelocity += movement.angularAcceleration * deltaTime_s;

                return true;
            }};

        manager.forEachComponents(forEachMovement);
    }
};
} // namespace System
