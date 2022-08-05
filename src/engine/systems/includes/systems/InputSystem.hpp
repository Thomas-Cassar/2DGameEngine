#pragma once

#include "components/InputComponent.hpp"
#include "ecs/ISystem.hpp"

class InputSystem : public ISystem {
public:
    void update(EntityManager& manager) override;

    static bool isKeyPressed(InputComponent const& input, int key);
};