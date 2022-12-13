#include "systems/CollisionSystem.hpp"
#include "components/BoxCollision.hpp"
#include "components/ColorComponent.hpp"
#include "components/MovementComponent.hpp"
#include "components/PlayerComponent.hpp"
#include "components/TransformComponent.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

static bool collisionCheck(TransformComponent const& transformA, BoxCollision const& boxA,
                           TransformComponent const& transformB, BoxCollision const& boxB)
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

// TODO: Update with more complex collision resolution
// Currently resolves collision between a movable and stationary object
static void resolveCollision(TransformComponent& transformA, MovementComponent& moveA, BoxCollision const& boxA,
                             TransformComponent& transformB, BoxCollision const& boxB, float deltaTime_s)
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
                return;
            }
        }
        // Default case we returned to the previous position using previous for loop
    }
}

void CollisionSystem::update(EntityManager& manager, float deltaTime_s)
{
    ComponentsForEachFn<BoxCollision, TransformComponent> const setCollidingFalse{
        [](Entity entity, BoxCollision& box, TransformComponent& transform) {
            box.colliding = false;
            return true;
        }};

    // Check collision for every player
    ComponentsForEachFn<BoxCollision, TransformComponent, PlayerComponent> const forEachPlayerCollision{
        [&manager, &deltaTime_s](Entity entityA, BoxCollision& boxA, TransformComponent& transformA,
                                 PlayerComponent& player) {
            ComponentsForEachFn<BoxCollision, TransformComponent> const forEachCollisionInner{
                [&manager, &entityA, &boxA, &transformA, &deltaTime_s](Entity entityB, BoxCollision& boxB,
                                                                       TransformComponent& transformB) {
                    // Don't check self collision
                    if (entityA == entityB)
                    {
                        return true;
                    }

                    if (collisionCheck(transformA, boxA, transformB, boxB))
                    {
                        boxA.colliding = true;
                        boxB.colliding = true;
                        if (manager.hasComponents<MovementComponent>(entityA))
                        {
                            MovementComponent& moveA{manager.getComponent<MovementComponent>(entityA)};
                            resolveCollision(transformA, moveA, boxA, transformB, boxB, deltaTime_s);
                        }
                    }

                    return true;
                }};

            manager.forEachComponents<BoxCollision, TransformComponent>(forEachCollisionInner);
            return true;
        }};

    manager.forEachComponents<BoxCollision, TransformComponent>(setCollidingFalse);
    manager.forEachComponents(forEachPlayerCollision);
}
