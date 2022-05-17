#include "input/Input.hpp"

Input::Input(GLFWwindow* window)
:
window(window)
{
}

void Input::updateInput()
{
    glfwPollEvents();
    prevMousePos=mousePos;
    glfwGetCursorPos(window,&mousePos.x,&mousePos.y);
    //If this is the first update call set previous mouse to starting so it does not jump from origin
    if(firstUpdate)
    {
        prevMousePos=mousePos;
        firstUpdate=false;
    }

    deltaMousePos.x=mousePos.x-prevMousePos.x;
    deltaMousePos.y=prevMousePos.y-mousePos.y;
}

glm::dvec2 Input::getMousePos()
{ return mousePos; }

glm::dvec2 Input::getPrevMousePos()
{ return prevMousePos; }

glm::dvec2 Input::getDeltaMousePos()
{ return deltaMousePos; }

int Input::getKeyState(int key)
{
    return glfwGetKey(window,key);
}

bool Input::isKeyPressed(int key)
{
    return glfwGetKey(window,key)==GLFW_PRESS;
}