module;
#include "glm//gtx/norm.hpp"
#include "glm/glm.hpp"

#include "glfw/glfw3.h"
export module System:PlayerSystem;

import Component;
import Ecs;
import :InputSystem;

namespace System
{

export class PlayerSystem : public Ecs::ISystem {
public:
    void update(Ecs::EntityManager& manager, float deltaTime_s) override
    {
        Ecs::ComponentsForEachFn<Component::PlayerComponent, Component::TransformComponent,
                                 Component::MovementComponent, Component::InputComponent, Component::CameraComponent,
                                 Component::BoxCollision> const forEachPlayer{
            [](Ecs::Entity entity, Component::PlayerComponent& player, Component::TransformComponent& transform,
                       Component::MovementComponent& move, Component::InputComponent& input,
                       Component::CameraComponent& cam, Component::BoxCollision& box) {
                movePlayer(move, cam, input, box, player);
                return true;
            }};

        manager.forEachComponents(forEachPlayer);
    }

private:
    static void movePlayer(Component::MovementComponent& move, Component::CameraComponent const& cam,
                           Component::InputComponent const& input, Component::BoxCollision const& box,Component::PlayerComponent& player)
    {
        // DEBUG CONTROLS
        if (InputSystem::isKeyPressedTransition(input, GLFW_KEY_R))
        {
            // Toggle to debug mode
            if (!player.debugControls)
            {
                move.doesGravity = false;
                move.velocity = {};
                move.acceleration = {};
                move.terminalVelocityAffectsVertical = true;
                player.debugControls = true;
            }
            else
            {
                move.doesGravity = true;
                move.terminalVelocityAffectsVertical = false;
                player.debugControls = false;
            }
        }

        glm::vec3 forwardVector{cam.cameraFront};
        forwardVector.y = 0.0F;
        forwardVector = glm::normalize(forwardVector);
        glm::vec3 rightVector{glm::normalize(glm::cross(forwardVector, cam.cameraUp))};
        glm::vec3 moveAcceleration{};

        // If we are using debug controls allow moving on y axis
        if (player.debugControls)
        {
            forwardVector=cam.cameraFront;
        }

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
            //If using debug controls also slow down on y axis
            glm::vec3 velocity=player.debugControls ? move.velocity : glm::vec3{move.velocity.x, 0.0f, move.velocity.z};
            if (glm::length2(velocity) > epsilon)
            {
                moveAcceleration = glm::normalize(velocity) * decelerationMag;
            }
            else
            {
                move.velocity.x = 0.0f;
                move.velocity.y = player.debugControls ? 0.0f : move.velocity.y;
                move.velocity.z = 0.0f;
            }
        }

        // Jump handling
        if (box.colliding && box.collisionAxis.y && InputSystem::isKeyPressedTransition(input, GLFW_KEY_SPACE))
        {
            move.velocity.y += jumpVelocity;
        }

        move.acceleration += moveAcceleration;
    }
};
} // namespace System
