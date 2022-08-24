#include "graphics/Graphics.hpp"

#include "GLFW/glfw3.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"
#include <chrono>
#include <iostream>
#include <queue>

#include "components/BoxCollision.hpp"
#include "components/CameraComponent.hpp"
#include "components/Common.hpp"
#include "components/InputComponent.hpp"
#include "components/MovementComponent.hpp"
#include "components/PlayerComponent.hpp"
#include "components/TransformComponent.hpp"
#include "ecs/EntityManager.hpp"
#include "ecs/SystemManager.hpp"
#include "systems/Common.hpp"
#include "systems/MeshSystem.hpp"

#ifdef _WIN32
extern "C" { // Use High Performance GPU
__declspec(dllexport) bool AmdPowerXpressRequestHighPerformance = true;
__declspec(dllexport) bool NvOptimusEnablement = true;
}
#endif

void error_callback(int error, const char* description) { std::cerr << "Error: " << description << std::endl; }

int main()
{
    {
        glfwSetErrorCallback(error_callback);

        // Start GLFW Library
        if (!glfwInit())
            return -1;

        char* const glslVersion{"#version 330"};
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

        // Setup debug UI
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io{ImGui::GetIO()};
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glslVersion);

        std::shared_ptr<EntityManager> manager{std::make_shared<EntityManager>()};
        SystemManager sysManager(manager);
        registerComponents(*manager);
        registerSystems(sysManager);

        Entity player{manager->createEntity()};
        manager->addComponent<CameraComponent>(player, {});
        manager->addComponent<TransformComponent>(player, {{0.0f, 4.0f, 0.0f}});
        manager->addComponent<InputComponent>(player, {window});
        manager->addComponent<MovementComponent>(player, {true, true, 5.0F});
        manager->addComponent<PlayerComponent>(player, {});
        manager->addComponent<BoxCollision>(player, {1.0f, 2.0f, 1.0f});

        Entity coloredCube1{MeshSystem::createCubeColored(*manager, {{0.0F, 0.0F, 0.0F}, {}, {5.0F, 1.0F, 5.0F}},
                                                          {1.0F, 1.0F, 1.0F, 1.0F})};
        constexpr int numCubes{100};
        for (int i{}; i < numCubes; i++)
        {
            MeshSystem::createCubeColored(
                *manager,
                {{static_cast<float>(rand()) / RAND_MAX * 100.0f, static_cast<float>(rand()) / RAND_MAX * 10.0f,
                  static_cast<float>(rand()) / RAND_MAX * 100.0f}},
                {1.0f, 0.0f, 0.0f, 0.0f});
        }

        std::chrono::time_point lastTime{std::chrono::steady_clock::now()};

        // Queue to average frame times
        std::queue<float> frameTimes;
        constexpr int frameTimesCount{400};
        float frameTimeSum{};
        for (int i{}; i < frameTimesCount; i++)
            frameTimes.push(0.0F);

        while (!glfwWindowShouldClose(window))
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            int winWidth, winHeight;
            glfwGetFramebufferSize(window, &winWidth, &winHeight);
            glViewport(0, 0, winWidth, winHeight);

            // Start new imgui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            std::chrono::time_point updateTime{std::chrono::steady_clock::now()};
            float deltaTime_s{std::chrono::duration<float>(updateTime - lastTime).count()};
            sysManager.updateSystems(deltaTime_s);
            lastTime = updateTime;

            {
                // Get and display the average FPS
                frameTimeSum += deltaTime_s;
                frameTimeSum -= frameTimes.front();
                frameTimes.pop();
                frameTimes.push(deltaTime_s);

                ImGui::Begin("Stats:");
                ImGui::Text(std::string("FPS: " + std::to_string(1.0F / (frameTimeSum / frameTimesCount))).c_str());
                ImGui::End();
            }

            // Render imgui frame
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
        }

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    glfwTerminate();

    return 0;
}