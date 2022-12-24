export module System:MovementSystem;

import Component;
import Ecs;

namespace System
{

export class MovementSystem : public Ecs::ISystem {
public:
    void update(Ecs::EntityManager& manager, float deltaTime_s) override;
};

} // namespace System
