export module System:MeshSystem;
import Ecs;
import Component;

namespace System
{

export class MeshSystem : public Ecs::ISystem {
public:
    void update(Ecs::EntityManager& manager, float deltaTime_s) override;
};

} // namespace System
