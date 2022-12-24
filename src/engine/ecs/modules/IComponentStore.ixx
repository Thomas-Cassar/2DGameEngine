export module Ecs:IComponentStore;

import :Entity;

namespace Ecs
{
using ComponentStoreId = size_t;

export class IComponentStore {
public:
    virtual ~IComponentStore() = default;
    virtual void destroyComponent(Entity entity) = 0;
};
} // namespace Ecs