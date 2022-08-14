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
            glm::vec3 deltaVelocity{movement.acceleration * deltaTime_s};
            translation.position += movement.velocity * deltaTime_s;
            movement.velocity += deltaVelocity;

            return true;
        }};

    manager.forEachComponents<MovementComponent, TranslationComponent>(forEachMovement);
}
