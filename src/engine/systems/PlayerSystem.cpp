#include "systems/PlayerSystem.hpp"
#include "components/BoxCollision.hpp"
#include "components/CameraComponent.hpp"
#include "components/InputComponent.hpp"
#include "components/MovementComponent.hpp"
#include "components/PlayerComponent.hpp"
#include "components/TransformComponent.hpp"
#include "systems/InputSystem.hpp"

static void movePlayer(MovementComponent& move, CameraComponent const& cam, InputComponent const& input,
                       BoxCollision const& box)
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
    constexpr float jumpVelocity{15.0f};
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
    if (box.colliding && box.collisionAxis.y && InputSystem::isKeyPressedDown(input, GLFW_KEY_SPACE))
    {
        move.velocity.y += jumpVelocity;
    }

    move.acceleration += moveAcceleration;
}

void PlayerSystem::update(EntityManager& manager, float deltaTime_s)
{
    ComponentsForEachFn<PlayerComponent, TransformComponent, MovementComponent, InputComponent, CameraComponent,
                        BoxCollision> const forEachPlayer{
        [&manager](Entity entity, PlayerComponent& player, TransformComponent& transform, MovementComponent& move,
                   InputComponent& input, CameraComponent& cam, BoxCollision& box) {
            movePlayer(move, cam, input, box);
            return true;
        }};

    manager.forEachComponents(forEachPlayer);
}
