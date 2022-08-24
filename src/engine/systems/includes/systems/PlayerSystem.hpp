#pragma once
#include "components/CameraComponent.hpp"
#include "components/InputComponent.hpp"
#include "components/MovementComponent.hpp"
#include "ecs/EntityManager.hpp"
#include "ecs/ISystem.hpp"

class PlayerSystem : public ISystem {
public:
    void update(EntityManager& manager, float deltaTime_s) override;

    static void move(MovementComponent& move, CameraComponent& cam, InputComponent& input);
};