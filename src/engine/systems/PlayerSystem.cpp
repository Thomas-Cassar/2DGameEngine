#include "systems/PlayerSystem.hpp"
#include "components/PlayerComponent.hpp"
#include "components/TransformComponent.hpp"
#include "systems/InputSystem.hpp"

void PlayerSystem::update(EntityManager& manager, float deltaTime_s)
{
    ComponentsForEachFn<PlayerComponent, TransformComponent, MovementComponent, InputComponent, CameraComponent> const
        forEachPlayer{[&manager](Entity entity, PlayerComponent& player, TransformComponent& transform,
                                 MovementComponent& move, InputComponent& input, CameraComponent& cam) {
            PlayerSystem::move(move, cam, input);
            return true;
        }};

    manager.forEachComponents<PlayerComponent, TransformComponent, MovementComponent, InputComponent, CameraComponent>(
        forEachPlayer);
}

void PlayerSystem::move(MovementComponent& move, CameraComponent& cam, InputComponent& input)
{
    glm::vec3 forwardVector{cam.cameraFront};
    forwardVector.y = 0.0F;
    forwardVector = glm::normalize(forwardVector);
    glm::vec3 rightVector{glm::normalize(glm::cross(forwardVector, cam.cameraUp))};
    glm::vec3 moveAcceleration{};

    if (InputSystem::isKeyPressedDown(input, GLFW_KEY_W))
    {
        moveAcceleration += forwardVector;
    }
    if (InputSystem::isKeyPressedDown(input, GLFW_KEY_S))
    {
        moveAcceleration -= forwardVector;
    }
    if (InputSystem::isKeyPressedDown(input, GLFW_KEY_D))
    {
        moveAcceleration += rightVector;
    }
    if (InputSystem::isKeyPressedDown(input, GLFW_KEY_A))
    {
        moveAcceleration -= rightVector;
    }

    /// TODO: Set acceleration magnitude somewhere else
    constexpr float accelerationMag{50.0f};
    constexpr float decelerationMag{-50.0f};
    constexpr float jumpMag{30.0f};
    constexpr float epsilon{1.0f};
    if (moveAcceleration != glm::vec3{})
    {
        moveAcceleration = glm::normalize(moveAcceleration) * accelerationMag;
    }
    // When not moving accelerate in direction opposite of velocity to slow down
    else
    {
        glm::vec3 horizontalVelocity{move.velocity.x, 0.0f, move.velocity.z};
        if (glm::length2(horizontalVelocity) > epsilon)
        {
            moveAcceleration = glm::normalize(horizontalVelocity) * decelerationMag;
        }
        else
        {
            move.velocity.x = 0.0f;
            move.velocity.z = 0.0f;
        }
    }

    // Jump handling
    if (InputSystem::isKeyPressedDown(input, GLFW_KEY_SPACE))
    {
        moveAcceleration.y += jumpMag;
    }

    move.acceleration += moveAcceleration;
}
