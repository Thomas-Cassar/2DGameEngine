#pragma once

class IComponentStore {
public:
    virtual ~IComponentStore() = default;
    virtual void destroyComponent(Entity entity) = 0;
};