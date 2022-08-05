#include "systems/InputSystem.hpp"
#include "GLFW/glfw3.h"

void InputSystem::update(EntityManager& manager)
{
    glfwPollEvents();
    ComponentForEachFn<InputComponent> const forEachInput{[](Entity entity, InputComponent& InputComponent) {
        InputComponent.prevMousePos = InputComponent.mousePos;
        glfwGetCursorPos(InputComponent.windowPtr, &InputComponent.mousePos.x, &InputComponent.mousePos.y);
        InputComponent.deltaMousePos.x = InputComponent.mousePos.x - InputComponent.prevMousePos.x;
        // Flip delta for Y so that moving mouse up is a positive value
        InputComponent.deltaMousePos.y = InputComponent.prevMousePos.y - InputComponent.mousePos.y;

        // If this is the first time updating we ignore the first delta as we do not have a previous mouse position
        if (InputComponent.firstPoll)
        {
            InputComponent.firstPoll = false;
            InputComponent.deltaMousePos = {0, 0};
            InputComponent.prevMousePos = InputComponent.mousePos;
        }

        // Only one input component for now return immediately
        return false;
    }};
    manager.forEachComponent<InputComponent>(forEachInput);
}

bool InputSystem::isKeyPressed(InputComponent const& input, int key) { return glfwGetKey(input.windowPtr, key); }