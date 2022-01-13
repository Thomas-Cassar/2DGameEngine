#include <iostream>
#include "Graphics.hpp"
#include "GLFW/glfw3.h"

void error_callback(int error, const char* description)
{
	std::cerr<<"Error: "<<description<<std::endl;
}

int main()
{
    
	glfwSetErrorCallback(error_callback);
    GLFWwindow* window;

    //Start GLFW Library
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create window and ensure creation
	window = glfwCreateWindow(640, 480, "2DGameEngine", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		std::cerr<<"Window not made!"<<std::endl;
		return -1;
	}
	//Make window current
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	//Test GLEW is working and output GL Version
	if (glewInit() != GLEW_OK)
		std::cout << "Error" << std::endl;
	std::cout << glGetString(GL_VERSION) << std::endl;

	//Set clear color
	glClearColor(1.0f,1.0f,1.0f,1.0f);

    while(!glfwWindowShouldClose(window))
    {
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
		glfwWaitEvents();
    }

    glfwTerminate();

    return 0;
}