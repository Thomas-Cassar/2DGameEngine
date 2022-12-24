export module System:PlayerSystem;

import Component;
import Ecs;

namespace System
{

export class PlayerSystem : public Ecs::ISystem {
public:
    void update(Ecs::EntityManager& manager, float deltaTime_s) override;

private:
    static void movePlayer(Component::MovementComponent& move, Component::CameraComponent const& cam,
                           Component::InputComponent const& input, Component::BoxCollision const& box,
                           Component::PlayerComponent& player);
};

} // namespace System
