#include "systems/MovementSystem.hpp"
#include "components/MovementComponent.hpp"
#include "components/TranslationComponent.hpp"

void MovementSystem::update(EntityManager& manager, float deltaTime_s)
{
    ComponentsForEachFn<MovementComponent, TranslationComponent> const forEachMovement{
        [&manager, &deltaTime_s](Entity entity, MovementComponent& movement, TranslationComponent& translation) {
            if (movement.doesGravity)
            {
                movement.acceleration = {0.0F, -9.81F, 0.0F};
            }
            // Velocities for this time step is approximated to oldVelocity + half deltaVelocity
            glm::vec3 deltaVelocity{movement.acceleration * deltaTime_s};
            translation.position += (movement.velocity + 0.5F * deltaVelocity) * deltaTime_s;
            movement.velocity += deltaVelocity;

            // Angular
            // Derivation: https://gamedev.stackexchange.com/questions/108920/applying-angular-velocity-to-quaternion
            glm::vec3 deltaAngularVelocity{movement.angularAcceleration * deltaTime_s};
            translation.rotation =
                (glm::quat(0.5F * (movement.angularVelocity + 0.5F * deltaAngularVelocity) * deltaTime_s) +
                 glm::quat()) *
                translation.rotation;
            movement.angularVelocity += deltaAngularVelocity;

            return true;
        }};

    manager.forEachComponents<MovementComponent, TranslationComponent>(forEachMovement);
}
