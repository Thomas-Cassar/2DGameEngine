module;
#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
export module System:CameraSystem;
import Ecs;
import Component;
import :InputSystem;

namespace System
{

/// TODO: Find better home for these values
constexpr float kSensitivity{0.001F};
constexpr float kFov{70.0F};
constexpr float kPitchMax{glm::half_pi<float>() - 0.01F};

export class CameraSystem : public Ecs::ISystem {
public:
    void update(Ecs::EntityManager& manager, float deltaTime_s) override
    {
        Ecs::ComponentsForEachFn<Component::CameraComponent, Component::TransformComponent,
                                 Component::InputComponent> const forEachCamera{
            [&deltaTime_s](Ecs::Entity entity, Component::CameraComponent& camera,
                                     Component::TransformComponent& transform, Component::InputComponent& input) {
                int width, height;
                glfwGetWindowSize(input.windowPtr, &width, &height);
                updateCameraProjectionMatrix(camera, kFov, static_cast<float>(width), static_cast<float>(height));

                if (input.mouseLock)
                {
                    turnCameraFromInput(camera, transform, input.deltaMousePos);
                }
                return true;
            }};
        manager.forEachComponents(forEachCamera);
    }

    static void turnCameraFromInput(Component::CameraComponent& camera, Component::TransformComponent& transform,
                                    glm::dvec2 const& deltaInput)
    {
        // Change in input x axis changes yaw and input y axis changes pitch
        camera.targetYaw_rad += static_cast<float>(deltaInput.x) * kSensitivity;
        camera.targetPitch_rad += static_cast<float>(deltaInput.y) * kSensitivity;

        camera.targetYaw_rad = fmod(camera.targetYaw_rad, glm::two_pi<float>());

        // Prevent from moving camera past looking straight up or down
        camera.targetPitch_rad = std::min(camera.targetPitch_rad, kPitchMax);
        camera.targetPitch_rad = std::max(camera.targetPitch_rad, -1.0F * kPitchMax);

        camera.cameraFront.x = cos(camera.targetYaw_rad) * cos(camera.targetPitch_rad);
        camera.cameraFront.y = sin(camera.targetPitch_rad);
        camera.cameraFront.z = sin(camera.targetYaw_rad) * cos(camera.targetPitch_rad);
        camera.cameraFront = glm::normalize(camera.cameraFront);
        transform.rotation = glm::quat({0.0F, -camera.targetYaw_rad, 0.0F});
    }

    static void updateCameraProjectionMatrix(Component::CameraComponent& camera, float const fov, float const width,
                                             float const height)
    {
        camera.proj = glm::perspective(glm::radians(fov), width / height, 0.1f, 1000.0f);
    }
};
} // namespace System
