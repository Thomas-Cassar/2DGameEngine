#include "systems/CollisionSystem.hpp"
#include "components/BoxCollision.hpp"
#include "components/ColorComponent.hpp"
#include "components/TranslationComponent.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

static bool collisionCheck(TranslationComponent const& translationA, BoxCollision const& boxA,
                           TranslationComponent const& translationB, BoxCollision const& boxB)
{
    // Based of of:
    // https://www.jkh.me/files/tutorials/Separating%20Axis%20Theorem%20for%20Oriented%20Bounding%20Boxes.pdf

    // Outer loop is collision box A
    glm::vec3 const& aP{translationA.position};
    glm::vec3 aX{glm::vec3(1.0F, 0.0F, 0.0F) * translationA.rotation};
    glm::vec3 aY{glm::vec3(0.0F, 1.0F, 0.0F) * translationA.rotation};
    glm::vec3 aZ{glm::vec3(0.0F, 0.0F, 1.0F) * translationA.rotation};
    float aW{boxA.width / 2.0F};
    float aH{boxA.height / 2.0F};
    float aD{boxA.depth / 2.0F};
    // Inner loop is collision box B
    glm::vec3 const& bP{translationB.position};
    glm::vec3 bX{glm::vec3(1.0F, 0.0F, 0.0F) * translationB.rotation};
    glm::vec3 bY{glm::vec3(0.0F, 1.0F, 0.0F) * translationB.rotation};
    glm::vec3 bZ{glm::vec3(0.0F, 0.0F, 1.0F) * translationB.rotation};
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

void CollisionSystem::update(EntityManager& manager, float deltaTime_s)
{
    ComponentsForEachFn<BoxCollision, TranslationComponent> const forEachCollisionOuter{
        [&manager](Entity entityA, BoxCollision& boxA, TranslationComponent& translationA) {
            ComponentsForEachFn<BoxCollision, TranslationComponent> const forEachCollisionInner{
                [&manager, &entityA, &boxA, &translationA](Entity entityB, BoxCollision& boxB,
                                                           TranslationComponent& translationB) {
                    // Don't check self collision
                    if (entityA == entityB)
                    {
                        return true;
                    }

                    if (collisionCheck(translationA, boxA, translationB, boxB))
                    {
                        // Do something
                        if (manager.hasComponents<ColorComponent>(entityA))
                        {
                            manager.getComponent<ColorComponent>(entityA).color = {1.0F, 0.0F, 0.0F, 1.0F};
                        }
                    }

                    return true;
                }};

            manager.forEachComponents<BoxCollision, TranslationComponent>(forEachCollisionInner);
            return true;
        }};

    manager.forEachComponents<BoxCollision, TranslationComponent>(forEachCollisionOuter);
}
