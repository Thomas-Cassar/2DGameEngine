export module System:TestSystem;

import Component;
import Ecs;

namespace System
{

export class TestSystem : public Ecs::ISystem {
public:
    void update(Ecs::EntityManager& manager, float deltaTime_s) override;
};

} // namespace System
