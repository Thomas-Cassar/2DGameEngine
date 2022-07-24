#pragma once
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include <unordered_map>

/**
 * @brief Class for handling GLFW input
 * 
 */
class Input {
private:
    /**The window that we receive input from*/
    GLFWwindow* window;
    /**True if this is the first update call*/
    bool firstUpdate=true;
    /**Current mouse position */
    glm::dvec2 mousePos=glm::dvec2(0.0,0.0);
    /**Mouse position during last update */
    glm::dvec2 prevMousePos=glm::dvec2(0.0,0.0);
    /**Change in mouse position since last update */
    glm::dvec2 deltaMousePos=glm::dvec2(0.0,0.0);

public:

    /**
     * @brief Construct a new Input object
     * 
     * @param window The window to receive input from
     */
    Input(GLFWwindow* window);

    /**
     * @brief Polls for GLFW input and updates input state
     * 
     */
    void updateInput();

    /**
     * @brief Get the Mouse Pos object
     * 
     * @return glm::dvec2 
     */
    glm::dvec2 getMousePos();

    /**
     * @brief Get the Prev Mouse Pos object
     * 
     * @return glm::dvec2 
     */
    glm::dvec2 getPrevMousePos();

    /**
     * @brief Get the Delta Mouse Pos object
     * 
     * @return glm::dvec2 
     */
    glm::dvec2 getDeltaMousePos();

    /**
     * @brief Get the state of a keyboard key
     * 
     * @param key Key to check (ex: GLFW_KEY_W)
     * @return int Key state (ex: GLFW_PRESS)
     */
    int getKeyState(int key);

    /**
     * @brief True if the key is GLFW_PRESS
     * 
     * @param key Key to check (ex: GLFW_KEY_W)
     * @return bool true if key is GLFW_PRESS false otherwise
     */
    bool isKeyPressed(int key);
};