#pragma once
#include "GLFW/glfw3.h"

#include "components/IComponent.hpp"
#include "glm/glm.hpp"
#include <memory>

struct InputComponent : public IComponent {
    GLFWwindow* windowPtr;
    /**Current mouse position */
    glm::dvec2 mousePos;
    /**Mouse position during last update */
    glm::dvec2 prevMousePos;
    /**Change in mouse position since last update */
    glm::dvec2 deltaMousePos;
    bool firstPoll = true;

    InputComponent(GLFWwindow* windowPtr) : windowPtr(windowPtr) {}
};