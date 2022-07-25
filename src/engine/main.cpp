#include "GLFW/glfw3.h"
#include "graphics/Graphics.hpp"
#include <iostream>

#include "graphics/Camera.hpp"
#include "graphics/IndexBuffer.hpp"
#include "graphics/Shader.hpp"
#include "graphics/Texture2D.hpp"
#include "graphics/VertexArray.hpp"
#include "graphics/VertexBuffer.hpp"
#include "graphics/VertexBufferLayout.hpp"

#include "input/Input.hpp"

#include "entity_component/Actor.hpp"
#include "entity_component/EntityManager.hpp"

#include "renderable/Plane2D.hpp"
#include "renderable/RenderableManager.hpp"

void error_callback(int error, const char *description) { std::cerr << "Error: " << description << std::endl; }

int main()
{
    {
        glfwSetErrorCallback(error_callback);
        GLFWwindow *window;

        // Start GLFW Library
        if (!glfwInit())
            return -1;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Create window and ensure creation
        window = glfwCreateWindow(640, 480, "2DGameEngine", NULL, NULL);
        if (!window)
        {
            glfwTerminate();
            std::cerr << "Window not made!" << std::endl;
            return -1;
        }
        // Make window current
        glfwMakeContextCurrent(window);
        glfwSwapInterval(0);

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        if (glfwRawMouseMotionSupported())
            glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

        // Test GLEW is working and output GL Version
        if (glewInit() != GLEW_OK)
            std::cout << "Error" << std::endl;
        std::cout << glGetString(GL_VERSION) << std::endl;

        // Set clear color
        glClearColor(0.5f, 1.0f, 1.0f, 1.0f);

        Input input(window);
        Camera camera(window);
        EntityManager objManager;
        RenderableManager rendManager;
        std::shared_ptr<Plane2D> testTexturedPlane{objManager.registerEntity<Plane2D>(5.0f, 5.0f, "assets/test.png")};
        std::shared_ptr<Plane2D> testColoredPlane{
            objManager.registerEntity<Plane2D>(1.0f, 1.0f, glm::vec3(0.5f, 0.8f, 1.0f))};
        rendManager.registerRenderable(testTexturedPlane);
        rendManager.registerRenderable(testColoredPlane);

        // Wireframe
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        while (!glfwWindowShouldClose(window))
        {
            glClear(GL_COLOR_BUFFER_BIT);
            testTexturedPlane->setYaw(testTexturedPlane->getYaw() + 0.1f);
            testColoredPlane->setTranslation(glm::vec3(fmod(testColoredPlane->getTranslation().x + 0.001f, 5.0f),
                                                       testColoredPlane->getTranslation().y,
                                                       testColoredPlane->getTranslation().z));
            rendManager.renderRenderables(camera.getProjMatrix() * camera.getViewMatrix());
            glfwSwapBuffers(window);

            input.updateInput();
            // Test input handling
            camera.turnCameraFromInput(input.getDeltaMousePos());
            if (input.isKeyPressed(GLFW_KEY_W))
                camera.moveForward(0.01f);
            if (input.isKeyPressed(GLFW_KEY_S))
                camera.moveForward(-0.01f);
            if (input.isKeyPressed(GLFW_KEY_D))
                camera.moveRight(0.01f);
            if (input.isKeyPressed(GLFW_KEY_A))
                camera.moveRight(-0.01f);
            if (input.isKeyPressed(GLFW_KEY_E))
                camera.moveY(0.01f);
            if (input.isKeyPressed(GLFW_KEY_Q))
                camera.moveY(-0.01f);
        }
    }
    glfwTerminate();

    return 0;
}