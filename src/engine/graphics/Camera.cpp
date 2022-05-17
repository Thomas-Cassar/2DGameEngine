#include "graphics/Camera.hpp"

Camera::Camera(GLFWwindow* window)
:
window(window),
cameraFront(
            (float)cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
	        (float)sin(glm::radians(pitch)),
	        (float)sin(glm::radians(yaw)) * cos(glm::radians(pitch)))
{
    int width,height;
    glfwGetWindowSize(window,&width,&height);
    proj = glm::perspective(glm::radians(70.0f), (float)width / (float)height, 0.1f, 1000.0f);
}

glm::vec3 Camera::getTranslation()
{
	return translation;
}

void Camera::setTranslation(glm::vec3 translation)
{
    this->translation=translation;
}

glm::mat4 Camera::getViewMatrix()
{
	glm::mat4 viewmatrix = glm::lookAt(
		translation,
		cameraFront+translation,
		cameraUp
	);

	return viewmatrix;
}

glm::mat4 Camera::getProjMatrix()
{
	return proj;
}

void Camera::moveForward(float distance)
{
	translation +=  distance * cameraFront;
}
void Camera::moveRight(float distance)
{
	translation += glm::normalize(glm::cross(cameraFront, cameraUp)) *  distance;
}
void Camera::move(glm::vec3 distance)
{
    translation+=distance;
}
void Camera::moveX(float distance)
{
    translation.x += distance;
}
void Camera::moveY(float distance)
{
    translation.y += distance;
}
void Camera::moveZ(float distance)
{
    translation.z += distance;
}


void Camera::turnCameraFromInput(glm::dvec2 deltaInput)
{   
    //Reference material
	//https://learnopengl.com/Getting-started/Camera
	
    //Scale input from sensitivity
	deltaInput*=sensitivity;

    //Change in input x axis changes yaw and input y axis changes pitch
	yaw += (float)deltaInput.x;
	pitch += (float)deltaInput.y;

    //Prevent from moving camera past looking straight up or down
	if (pitch > 89.0)
		pitch = 89.0;
	if (pitch < -89.0)
		pitch = -89.0;

    yaw = fmod(yaw,360.0f);

	cameraFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront.y = sin(glm::radians(pitch));
	cameraFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(cameraFront);
}
