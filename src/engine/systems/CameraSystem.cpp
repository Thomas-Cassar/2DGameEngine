#include "GLFW/glfw3.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

#include "components/InputComponent.hpp"
#include "systems/CameraSystem.hpp"
#include "systems/InputSystem.hpp"

/// TODO: Find better home for these values
constexpr float kSensitivity{0.001F};
constexpr float kFov{70.0F};
constexpr float kPitchMax{glm::half_pi<float>() - 0.01F};

void CameraSystem::update(EntityManager& manager, float deltaTime_s)
{
    InputComponent const* input;
    ComponentsForEachFn<InputComponent> const forEachInput{[&input](Entity entity, InputComponent& InputComponent) {
        input = &InputComponent;
        // Only one input component for now return immediately
        return false;
    }};
    manager.forEachComponents<InputComponent>(forEachInput);

    ComponentsForEachFn<CameraComponent, TranslationComponent> const forEachCamera{
        [&manager, &input, &deltaTime_s](Entity entity, CameraComponent& camera, TranslationComponent& translation) {
            if (input == nullptr)
            {
                return false;
            }

            int width, height;
            glfwGetWindowSize(input->windowPtr, &width, &height);
            updateCameraProjectionMatrix(camera, kFov, static_cast<float>(width), static_cast<float>(height));

            if (input->mouseLock)
            {
                turnCameraFromInput(camera, translation, input->deltaMousePos);
            }

            // Update camera position
            if (InputSystem::isKeyPressedDown(*input, GLFW_KEY_W))
                moveForward(camera, translation, 10.0f * deltaTime_s);
            if (InputSystem::isKeyPressedDown(*input, GLFW_KEY_S))
                moveForward(camera, translation, -10.0f * deltaTime_s);
            if (InputSystem::isKeyPressedDown(*input, GLFW_KEY_D))
                moveRight(camera, translation, 10.0f * deltaTime_s);
            if (InputSystem::isKeyPressedDown(*input, GLFW_KEY_A))
                moveRight(camera, translation, -10.0f * deltaTime_s);
            return true;
        }};
    manager.forEachComponents<CameraComponent, TranslationComponent>(forEachCamera);
}

void CameraSystem::moveForward(CameraComponent const& camera, TranslationComponent& translation, float distance)
{
    translation.position += distance * camera.cameraFront;
}

void CameraSystem::moveRight(CameraComponent const& camera, TranslationComponent& translation, float distance)
{
    translation.position += distance * glm::normalize(glm::cross(camera.cameraFront, camera.cameraUp));
}

void CameraSystem::turnCameraFromInput(CameraComponent& camera, TranslationComponent& translation,
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
    translation.rotation = glm::quat({camera.targetPitch_rad, camera.targetYaw_rad, 0.0F});
}

void CameraSystem::updateCameraProjectionMatrix(CameraComponent& camera, float const fov, float const width,
                                                float const height)
{
    camera.proj = glm::perspective(glm::radians(fov), width / height, 0.1f, 1000.0f);
}
