export module Ecs:ISystem;
import :EntityManager;

namespace Ecs
{
export class ISystem {
public:
    virtual ~ISystem() = default;
    virtual void update(EntityManager& manager, float deltaTime_s) = 0;
};
} // namespace Ecs