#include "systems/PlayerSystem.hpp"
#include "glm//gtx/norm.hpp"
#include "glm/glm.hpp"
#include "systems/InputSystem.hpp"

import Component;

#include "glfw/glfw3.h"

static void movePlayer(Component::MovementComponent& move, Component::CameraComponent const& cam,
                       Component::InputComponent const& input, Component::BoxCollision const& box)
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
    ComponentsForEachFn<Component::PlayerComponent, Component::TransformComponent, Component::MovementComponent,
                        Component::InputComponent, Component::CameraComponent, Component::BoxCollision> const
        forEachPlayer{[&manager](Entity entity, Component::PlayerComponent& player,
                                 Component::TransformComponent& transform, Component::MovementComponent& move,
                                 Component::InputComponent& input, Component::CameraComponent& cam,
                                 Component::BoxCollision& box) {
            movePlayer(move, cam, input, box);
            return true;
        }};

    manager.forEachComponents(forEachPlayer);
}
