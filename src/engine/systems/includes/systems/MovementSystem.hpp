#pragma once

#include "ecs/EntityManager.hpp"
#include "ecs/ISystem.hpp"

class MovementSystem : public ISystem {
public:
    void update(EntityManager& manager, float deltaTime_s) override;
};