#include "graphics/Graphics.hpp"

#include "GLFW/glfw3.h"
#include <iostream>

#include "components/CameraComponent.hpp"
#include "components/Common.hpp"
#include "components/InputComponent.hpp"
#include "components/TranslationComponent.hpp"
#include "ecs/EntityManager.hpp"
#include "ecs/SystemManager.hpp"
#include "systems/Common.hpp"
#include "systems/MeshSystem.hpp"

void error_callback(int error, const char* description) { std::cerr << "Error: " << description << std::endl; }

int main()
{
    {
        glfwSetErrorCallback(error_callback);

        // Start GLFW Library
        if (!glfwInit())
            return -1;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Create window and ensure creation
        GLFWwindow* window{glfwCreateWindow(640, 480, "2DGameEngine", NULL, NULL)};

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
        {
            std::cerr << "Could not start GLEW" << std::endl;
            return -1;
        }
        std::cout << glGetString(GL_VERSION) << std::endl;

        // Set clear color
        glClearColor(0.5f, 1.0f, 1.0f, 1.0f);

        // Wireframe
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        // Cull back faces
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        // Enable depth testing
        glEnable(GL_DEPTH_TEST);

        std::shared_ptr<EntityManager> manager{std::make_shared<EntityManager>()};
        SystemManager sysManager(manager);
        registerComponents(*manager);
        registerSystems(sysManager);

        Entity camera{manager->createEntity()};
        manager->addComponent<CameraComponent>(camera, {});
        manager->addComponent<TranslationComponent>(camera, {});
        Entity input{manager->createEntity()};
        manager->addComponent<InputComponent>(input, {window});
        MeshSystem::createCubeColored(*manager, {{5.0F, 0.0F, 0.0F}}, {1.0F, 0.0F, 0.0F});
        MeshSystem::createCubeColored(*manager, {{0.0F, 0.0F, 5.0F}}, {0.0F, 1.0F, 0.0F});
        MeshSystem::createCubeColored(*manager, {{-5.0F, 0.0F, 0.0F}}, {0.0F, 0.0F, 1.0F});
        MeshSystem::createCubeTextured(*manager, {{0.0F, 0.0F, -5.0F}}, "assets/test.png");

        while (!glfwWindowShouldClose(window))
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            sysManager.updateSystems();
            glfwSwapBuffers(window);
        }
    }

    glfwTerminate();

    return 0;
}