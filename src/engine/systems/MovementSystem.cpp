module;
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
module System:MovementSystem;

import Component;
import Ecs;

namespace System
{
// Value to use for gravity for objects that experience gravity
constexpr float gravity{-20.0f};

void MovementSystem::update(Ecs::EntityManager& manager, float deltaTime_s)
{
    Ecs::ComponentsForEachFn<Component::MovementComponent, Component::TransformComponent> const forEachMovement{
        [&deltaTime_s](Ecs::Entity entity, Component::MovementComponent& movement,
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

            // Clamp to terminal velocity
            glm::vec3 velocity = movement.terminalVelocityAffectsVertical
                                     ? movement.velocity
                                     : glm::vec3{movement.velocity.x, 0.0f, movement.velocity.z};
            if (glm::length(velocity) > movement.terminalVelocity)
            {
                velocity = glm::normalize(velocity) * movement.terminalVelocity;
            }

            movement.velocity = movement.terminalVelocityAffectsVertical
                                    ? velocity
                                    : glm::vec3{velocity.x, movement.velocity.y, velocity.z};

            transform.position += movement.velocity * deltaTime_s;

            // Reset acceleration so forces can be applied again during the next frame
            movement.acceleration = {};

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
} // namespace System
