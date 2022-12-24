module;
#include "GLFW/glfw3.h"
export module System:InputSystem;

import Ecs;
import Component;

namespace System
{

export class InputSystem : public Ecs::ISystem {
public:
    void update(Ecs::EntityManager& manager, float deltaTime_s) override;

    static bool isKeyPressedDown(Component::InputComponent const& input, int key);

    static bool isKeyPressedTransition(Component::InputComponent const& input, int key);

private:
    static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};

} // namespace System
