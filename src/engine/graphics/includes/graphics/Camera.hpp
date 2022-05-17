#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glfw/glfw3.h"

class Camera {
private:
    /**Postion of the camera*/
	glm::vec3 translation=glm::vec3(0.0f,0.0f, 5.0f);
    /**Point that camera is looking at (normalized)*/
	glm::vec3 cameraFront;
    /**Vector that points to up direction*/
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    /**Camera projection matrix*/
	glm::mat4 proj;
    /**Yaw of camera*/
	float yaw=270.0f; 
    /**Pitch of camera*/
    float pitch=0.0f;
    /**Value to scale turning from input by*/
	float sensitivity=.1f;
    /**Window this camera is a part of */
	GLFWwindow* window;

public:
    /**
     * @brief Construct a new Camera object
     * 
     * @param window The window that this camera is in (Used for calculations based on window size)
     */
	Camera(GLFWwindow* window);
    /**
     * @brief Get the position of the camera
     * 
     * @return glm::vec3 Vector containing camera position
     */
	glm::vec3 getTranslation();
    /**
     * @brief Set the position of the camera
     * 
     * @param translation Value to set translation to
     */
    void setTranslation(glm::vec3 translation);
    /**
     * @brief Get the View Matrix for the camera
     * 
     * @return glm::mat4 Camera view matrix
     */
	glm::mat4 getViewMatrix();
    /**
     * @brief Get the Projection Matrix for the camera
     * 
     * @return glm::mat4 Camera projection matrix
     */
	glm::mat4 getProjMatrix();
    /**
     * @brief Moves camera forward or backwards based on value specified (with respect to what the camera is looking at)
     * 
     * @param distance Distance to move camera forward or backwards(negative)
     */
	void moveForward(float distance);
    /**
     * @brief Moves camera right or left based on value specified (with respect to what the camera is looking at)
     * 
     * @param distance Distance to move camera right or left(negative)
     */
	void moveRight(float distance);
    /**
     * @brief Moves the camera based on the vector specified
     * 
     * @param distance Distance to move camera
     */
	void move(glm::vec3 distance);
    /**
     * @brief Moves camera in X axis based on value specified
     * 
     * @param distance Distance to move camera in X axis
     */
	void moveX(float distance);
    /**
     * @brief Moves camera in Y axis based on value specified
     * 
     * @param distance Distance to move camera in Y axis
     */
	void moveY(float distance);
    /**
     * @brief Moves camera in Z axis based on value specified
     * 
     * @param distance Distance to move camera in Z axis
     */
	void moveZ(float distance);
    /**
     * @brief Changes what the camera is pointing towards based on user input
     * 
     * @param deltaInput The change in input that corresponds to the change in camera direction
     */
	void turnCameraFromInput(glm::dvec2 deltaInput);
};
	
