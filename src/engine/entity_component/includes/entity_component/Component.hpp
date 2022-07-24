#pragma once
#include "entity_component/ComponentType.hpp"

class Component
{
public:
    Component(ComponentType type);
    virtual ~Component() = default;

private:
    ComponentType componentType;
};