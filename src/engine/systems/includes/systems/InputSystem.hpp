#pragma once

#include "components/InputComponent.hpp"
#include "ecs/ISystem.hpp"

#include <utility>

class InputSystem : public ISystem {
public:
    void update(EntityManager& manager) override;

    static bool isKeyPressedDown(InputComponent const& input, int key);
    static bool isKeyPressedTransition(InputComponent const& input, int key);

private:
    static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};