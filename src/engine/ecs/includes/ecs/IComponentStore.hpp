#pragma once

using ComponentStoreId = size_t;

class IComponentStore {
public:
    virtual ~IComponentStore() = default;
    virtual void destroyComponent(Entity entity) = 0;
};