module;
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include <algorithm>
module System:CollisionSystem;

import Ecs;
import Component;

namespace System
{

void CollisionSystem::update(Ecs::EntityManager& manager, float deltaTime_s)
{
    Ecs::ComponentsForEachFn<Component::BoxCollision, Component::TransformComponent> const setCollidingFalse{
        [](Ecs::Entity entity, Component::BoxCollision& box, Component::TransformComponent& transform) {
            box.colliding = false;
            return true;
        }};

    // Check collision for every player
    Ecs::ComponentsForEachFn<Component::BoxCollision, Component::TransformComponent, Component::PlayerComponent> const
        forEachPlayerCollision{[&manager, &deltaTime_s](Ecs::Entity entityA, Component::BoxCollision& boxA,
                                                        Component::TransformComponent& transformA,
                                                        Component::PlayerComponent& player) {
            Ecs::ComponentsForEachFn<Component::BoxCollision, Component::TransformComponent> const
                forEachCollisionInner{[&manager, &entityA, &boxA, &transformA,
                                       &deltaTime_s](Ecs::Entity entityB, Component::BoxCollision& boxB,
                                                     Component::TransformComponent& transformB) {
                    // Don't check self collision
                    if (entityA == entityB)
                    {
                        return true;
                    }

                    if (collisionCheck(transformA, boxA, transformB, boxB))
                    {
                        boxA.colliding = true;
                        boxB.colliding = true;
                        if (!boxB.canOverlap && manager.hasComponents<Component::MovementComponent>(entityA))
                        {
                            Component::MovementComponent& moveA{
                                manager.getComponent<Component::MovementComponent>(entityA)};
                            resolveCollision(transformA, moveA, boxA, transformB, boxB, deltaTime_s);
                        }
                    }

                    return true;
                }};

            manager.forEachComponents(forEachCollisionInner);
            return true;
        }};

    manager.forEachComponents(setCollidingFalse);
    manager.forEachComponents(forEachPlayerCollision);
}

bool CollisionSystem::collisionCheck(Component::TransformComponent const& transformA,
                                     Component::BoxCollision const& boxA,
                                     Component::TransformComponent const& transformB,
                                     Component::BoxCollision const& boxB)
{
    float maxA = std::max(boxA.height, std::max(boxA.width, boxA.depth));
    float maxB = std::max(boxB.height, std::max(boxB.width, boxB.depth));
    if (glm::length(transformA.position - transformB.position) > maxA + maxB)
    {
        return false;
    }
    // Based of of:
    // https://www.jkh.me/files/tutorials/Separating%20Axis%20Theorem%20for%20Oriented%20Bounding%20Boxes.pdf

    // Outer loop is collision box A
    glm::vec3 const& aP{transformA.position};
    glm::vec3 aX{glm::vec3(1.0F, 0.0F, 0.0F) * transformA.rotation};
    glm::vec3 aY{glm::vec3(0.0F, 1.0F, 0.0F) * transformA.rotation};
    glm::vec3 aZ{glm::vec3(0.0F, 0.0F, 1.0F) * transformA.rotation};
    float aW{boxA.width / 2.0F};
    float aH{boxA.height / 2.0F};
    float aD{boxA.depth / 2.0F};
    // Inner loop is collision box B
    glm::vec3 const& bP{transformB.position};
    glm::vec3 bX{glm::vec3(1.0F, 0.0F, 0.0F) * transformB.rotation};
    glm::vec3 bY{glm::vec3(0.0F, 1.0F, 0.0F) * transformB.rotation};
    glm::vec3 bZ{glm::vec3(0.0F, 0.0F, 1.0F) * transformB.rotation};
    float bW{boxB.width / 2.0F};
    float bH{boxB.height / 2.0F};
    float bD{boxB.depth / 2.0F};

    glm::vec3 T{bP - aP};

    // Face cases A:
    // Case 1:
    // L=aX
    if (abs(glm::dot(T, aX)) >
        aW + abs(bW * glm::dot(aX, bX)) + abs(bH * glm::dot(aX, bY)) + abs(bD * glm::dot(aX, bZ)))
    {
        return false;
    }
    // Case 2:
    // L=aY
    if (abs(glm::dot(T, aY)) >
        aH + abs(bW * glm::dot(aY, bX)) + abs(bH * glm::dot(aY, bY)) + abs(bD * glm::dot(aY, bZ)))
    {
        return false;
    }
    // Case 3:
    // L=aZ
    if (abs(glm::dot(T, aZ)) >
        aD + abs(bW * glm::dot(aZ, bX)) + abs(bH * glm::dot(aZ, bY)) + abs(bD * glm::dot(aZ, bZ)))
    {
        return false;
    }
    // Face cases B
    // Case 4:
    // L=bX
    if (abs(glm::dot(T, bX)) >
        bW + abs(aW * glm::dot(aX, bX)) + abs(aH * glm::dot(aY, bX)) + abs(aD * glm::dot(aZ, bX)))
    {
        return false;
    }
    // Case 5:
    // L=bY
    if (abs(glm::dot(T, bY)) >
        bH + abs(aW * glm::dot(aX, bY)) + abs(aH * glm::dot(aY, bY)) + abs(aD * glm::dot(aZ, bY)))
    {
        return false;
    }
    // Case 6:
    // L=bZ
    if (abs(glm::dot(T, bZ)) >
        bD + abs(aW * glm::dot(aX, bZ)) + abs(aH * glm::dot(aY, bZ)) + abs(aD * glm::dot(aZ, bZ)))
    {
        return false;
    }
    // Edge cases
    // Case 7:
    // L=aX X bX
    if (abs(glm::dot(T, glm::cross(aX, bX))) > abs(aH * glm::dot(aZ, bX)) + abs(aD * glm::dot(aY, bX)) +
                                                   abs(bH * glm::dot(aX, bZ)) + abs(bD * glm::dot(aX, bY)))
    {
        return false;
    }
    // Case 8:
    // L=aX X bY
    if (abs(glm::dot(T, glm::cross(aX, bY))) > abs(aH * glm::dot(aZ, bY)) + abs(aD * glm::dot(aY, bY)) +
                                                   abs(bW * glm::dot(aX, bZ)) + abs(bD * glm::dot(aX, bX)))
    {
        return false;
    }
    // Case 9:
    // L=aX X bz
    if (abs(glm::dot(T, glm::cross(aX, bZ))) > abs(aH * glm::dot(aZ, bZ)) + abs(aD * glm::dot(aY, bZ)) +
                                                   abs(bW * glm::dot(aX, bY)) + abs(bH * glm::dot(aX, bX)))
    {
        return false;
    }
    // Case 10:
    // L=aY X bX
    if (abs(glm::dot(T, glm::cross(aY, bX))) > abs(aW * glm::dot(aZ, bX)) + abs(aD * glm::dot(aX, bX)) +
                                                   abs(bH * glm::dot(aY, bZ)) + abs(bD * glm::dot(aY, bY)))
    {
        return false;
    }
    // Case 11:
    // L=aY X bY
    if (abs(glm::dot(T, glm::cross(aY, bY))) > abs(aW * glm::dot(aZ, bY)) + abs(aD * glm::dot(aX, bY)) +
                                                   abs(bW * glm::dot(aY, bZ)) + abs(bD * glm::dot(aY, bX)))
    {
        return false;
    }
    // Case 12:
    // L=aY X bZ
    if (abs(glm::dot(T, glm::cross(aY, bZ))) > abs(aW * glm::dot(aZ, bZ)) + abs(aD * glm::dot(aX, bZ)) +
                                                   abs(bW * glm::dot(aY, bY)) + abs(bH * glm::dot(aY, bX)))
    {
        return false;
    }
    // Case 13:
    // L=aZ X bX
    if (abs(glm::dot(T, glm::cross(aZ, bX))) > abs(aW * glm::dot(aY, bX)) + abs(aH * glm::dot(aX, bX)) +
                                                   abs(bH * glm::dot(aZ, bZ)) + abs(bD * glm::dot(aZ, bY)))
    {
        return false;
    }
    // Case 14:
    // L=aZ X bY
    if (abs(glm::dot(T, glm::cross(aZ, bY))) > abs(aW * glm::dot(aY, bY)) + abs(aH * glm::dot(aX, bY)) +
                                                   abs(bW * glm::dot(aZ, bZ)) + abs(bD * glm::dot(aZ, bX)))
    {
        return false;
    }
    // Case 15:
    // L=aZ X bZ
    if (abs(glm::dot(T, glm::cross(aZ, bZ))) > abs(aW * glm::dot(aY, bZ)) + abs(aH * glm::dot(aX, bZ)) +
                                                   abs(bW * glm::dot(aZ, bY)) + abs(bH * glm::dot(aZ, bX)))
    {
        return false;
    }

    // No axis separating collision boxes so collision occurs
    return true;
}

void CollisionSystem::resolveCollision(Component::TransformComponent& transformA, Component::MovementComponent& moveA,
                                       Component::BoxCollision& boxA, Component::TransformComponent& transformB,
                                       Component::BoxCollision& boxB, float deltaTime_s)
{
    if (moveA.canMove)
    {
        constexpr int collisionChecks{4};
        constexpr float collisionCheckStep{1.0F / static_cast<float>(collisionChecks)};
        glm::vec3 const oldPosition{transformA.position};
        // Epsilon value to multiply collision check by to get slightly past the collision
        constexpr float epsilon{1.03F};
        // y
        for (int i{1}; i <= collisionChecks; i++)
        {
            transformA.position.y -= (moveA.velocity.y * deltaTime_s * collisionCheckStep * epsilon);
            if (!collisionCheck(transformA, boxA, transformB, boxB))
            {
                moveA.velocity.y = 0.0f;
                boxA.collisionAxis = {false, true, false};
                return;
            }
        }
        transformA.position = oldPosition;
        // x
        for (int i{1}; i <= collisionChecks; i++)
        {
            transformA.position.x -= (moveA.velocity.x * deltaTime_s * collisionCheckStep * epsilon);
            if (!collisionCheck(transformA, boxA, transformB, boxB))
            {
                moveA.velocity.x = 0.0f;
                boxA.collisionAxis = {true, false, false};
                return;
            }
        }
        // z
        transformA.position = oldPosition;
        for (int i{1}; i <= collisionChecks; i++)
        {
            transformA.position.z -= (moveA.velocity.z * deltaTime_s * collisionCheckStep * epsilon);
            if (!collisionCheck(transformA, boxA, transformB, boxB))
            {
                moveA.velocity.z = 0.0f;
                boxA.collisionAxis = {false, false, true};
                return;
            }
        }
        // yx
        transformA.position = oldPosition;
        for (int i{1}; i <= collisionChecks; i++)
        {
            transformA.position.y -= (moveA.velocity.y * deltaTime_s * collisionCheckStep * epsilon);
            transformA.position.x -= (moveA.velocity.x * deltaTime_s * collisionCheckStep * epsilon);
            if (!collisionCheck(transformA, boxA, transformB, boxB))
            {
                moveA.velocity.y = 0.0f;
                moveA.velocity.x = 0.0f;
                boxA.collisionAxis = {true, true, false};
                return;
            }
        }
        // yz
        transformA.position = oldPosition;
        for (int i{1}; i <= collisionChecks; i++)
        {
            transformA.position.y -= (moveA.velocity.y * deltaTime_s * collisionCheckStep * epsilon);
            transformA.position.z -= (moveA.velocity.z * deltaTime_s * collisionCheckStep * epsilon);
            if (!collisionCheck(transformA, boxA, transformB, boxB))
            {
                moveA.velocity.y = 0.0f;
                moveA.velocity.z = 0.0f;
                boxA.collisionAxis = {false, true, true};
                return;
            }
        }
        // xz
        transformA.position = oldPosition;
        for (int i{1}; i <= collisionChecks; i++)
        {
            transformA.position.x -= (moveA.velocity.x * deltaTime_s * collisionCheckStep * epsilon);
            transformA.position.z -= (moveA.velocity.z * deltaTime_s * collisionCheckStep * epsilon);
            if (!collisionCheck(transformA, boxA, transformB, boxB))
            {
                moveA.velocity.x = 0.0f;
                moveA.velocity.z = 0.0f;
                boxA.collisionAxis = {true, false, true};
                return;
            }
        }
        // xyz
        transformA.position = oldPosition;
        for (int i{1}; i <= collisionChecks; i++)
        {
            transformA.position -= (moveA.velocity * deltaTime_s * collisionCheckStep * epsilon);
            if (!collisionCheck(transformA, boxA, transformB, boxB))
            {
                moveA.velocity = {};
                boxA.collisionAxis = {true, true, true};
                return;
            }
        }
        // Default case we returned to the previous position using previous for loop
    }
}

} // namespace System
