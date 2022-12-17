module;
#include "glfw/glfw3.h"
#include "glm/glm.hpp"
#include <memory>
#include <unordered_map>
export module Component:InputComponent;

import :IComponent;

namespace Component
{

export struct InputComponent : public IComponent {
    GLFWwindow* windowPtr;
    /**Current mouse position */
    glm::dvec2 mousePos;
    /**Mouse position during last update */
    glm::dvec2 prevMousePos;
    /**Change in mouse position since last update */
    glm::dvec2 deltaMousePos;
    std::shared_ptr<std::unordered_map<int, int>> keyStates;
    bool firstPoll = true;
    bool mouseLock = true;

    InputComponent(GLFWwindow* windowPtr)
        : windowPtr(windowPtr), keyStates(std::make_shared<std::unordered_map<int, int>>())
    {
    }
};
} // namespace Component
