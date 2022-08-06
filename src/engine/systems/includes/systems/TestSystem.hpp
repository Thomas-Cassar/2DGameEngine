#pragma once

#include "components/TranslationComponent.hpp"
#include "ecs/EntityManager.hpp"
#include "ecs/ISystem.hpp"

class TestSystem : public ISystem {
public:
    void update(EntityManager& manager) override;
};