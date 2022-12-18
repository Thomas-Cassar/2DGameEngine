module;
#include "GLFW/glfw3.h"
export module System:InputSystem;

import <unordered_map>;
import <utility>;
import Ecs;
import Component;

namespace System
{

export class InputSystem : public Ecs::ISystem {
public:
    void update(Ecs::EntityManager& manager, float deltaTime_s) override
    {
        // Clear previous callbacks
        glfwPollEvents();

        Ecs::ComponentsForEachFn<Component::InputComponent> const forEachInput{
            [](Ecs::Entity entity, Component::InputComponent& input) {
                input.prevMousePos = input.mousePos;
                glfwGetCursorPos(input.windowPtr, &input.mousePos.x, &input.mousePos.y);
                input.deltaMousePos.x = input.mousePos.x - input.prevMousePos.x;
                // Flip delta for Y so that moving mouse up is a positive value
                input.deltaMousePos.y = input.prevMousePos.y - input.mousePos.y;

                // If this is the first time updating we ignore the first delta as we do not have a previous mouse
                // position Also we set the key callback
                if (input.firstPoll)
                {
                    // InputComponent and window are mapped together and the window contains the map to store key
                    // transitions
                    glfwSetWindowUserPointer(input.windowPtr, input.keyStates.get());
                    glfwSetKeyCallback(input.windowPtr, InputSystem::glfwKeyCallback);
                    input.firstPoll = false;
                    input.deltaMousePos = {0, 0};
                    input.prevMousePos = input.mousePos;
                }

                if (isKeyPressedTransition(input, GLFW_KEY_ESCAPE) && input.mouseLock)
                {
                    input.mouseLock = false;
                    glfwSetInputMode(input.windowPtr, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                }
                else if (isKeyPressedTransition(input, GLFW_KEY_ESCAPE) && !input.mouseLock)
                {
                    input.mouseLock = true;
                    glfwSetInputMode(input.windowPtr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                }

                input.keyStates->clear();
                // Only one input component for now return immediately
                return false;
            }};
        manager.forEachComponents(forEachInput);
    }

    static bool isKeyPressedDown(Component::InputComponent const& input, int key)
    {
        return glfwGetKey(input.windowPtr, key) == GLFW_PRESS;
    }

    static bool isKeyPressedTransition(Component::InputComponent const& input, int key)
    {
        auto const& state{input.keyStates->find(key)};
        if (state == std::end(*input.keyStates))
        {
            return false;
        }
        return state->second == GLFW_PRESS;
    }

private:
    static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        static_cast<std::unordered_map<int, int>*>(glfwGetWindowUserPointer(window))->insert_or_assign(key, action);
    }
};
} // namespace System
