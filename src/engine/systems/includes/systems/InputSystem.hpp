#pragma once

#include "GLFW/glfw3.h"
#include "ecs/ISystem.hpp"

import Component;
#include <utility>

class InputSystem : public ISystem {
public:
    void update(EntityManager& manager, float deltaTime_s) override;

    static bool isKeyPressedDown(Component::InputComponent const& input, int key);
    static bool isKeyPressedTransition(Component::InputComponent const& input, int key);

private:
    static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};