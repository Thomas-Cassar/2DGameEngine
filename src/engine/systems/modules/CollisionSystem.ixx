export module System:CollisionSystem;

import Ecs;
import Component;

namespace System
{

export class CollisionSystem : public Ecs::ISystem {
public:
    void update(Ecs::EntityManager& manager, float deltaTime_s) override;

private:
    static bool collisionCheck(Component::TransformComponent const& transformA, Component::BoxCollision const& boxA,
                               Component::TransformComponent const& transformB, Component::BoxCollision const& boxB);

    // TODO: Update with more complex collision resolution
    // Currently resolves collision between a movable and stationary object
    static void resolveCollision(Component::TransformComponent& transformA, Component::MovementComponent& moveA,
                                 Component::BoxCollision& boxA, Component::TransformComponent& transformB,
                                 Component::BoxCollision& boxB, float deltaTime_s);
};

} // namespace System
