#pragma once
#include <memory>

#include "ecs/EntityManager.hpp"

class ISystem {
public:
    virtual ~ISystem() = default;
    virtual void update(EntityManager& manager) = 0;
};