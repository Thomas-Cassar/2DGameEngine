#pragma once
#include "ecs/EntityManager.hpp"
#include "ecs/ISystem.hpp"

class TestSystem : public ISystem {
public:
    void update(EntityManager& manager, float deltaTime_s) override;
};