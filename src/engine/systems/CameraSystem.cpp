#include "GLFW/glfw3.h"

#include "glm/gtc/matrix_transform.hpp"

#include "components/InputComponent.hpp"
#include "systems/CameraSystem.hpp"
#include "systems/InputSystem.hpp"

/// TODO: Find better home for these values
constexpr float kSensitivity{0.1F};
constexpr float kFov{70.0F};
constexpr float kPitchMax{89.99F};

void CameraSystem::update(EntityManager& manager)
{
    InputComponent const* input;
    ComponentForEachFn<InputComponent> const forEachInput{[&input](Entity entity, InputComponent& InputComponent) {
        input = &InputComponent;
        // Only one input component for now return immediately
        return false;
    }};
    manager.forEachComponent<InputComponent>(forEachInput);

    ComponentForEachFn<CameraComponent> const forEachCamera{[&manager, &input](Entity entity, CameraComponent& camera) {
        if (input == nullptr)
        {
            return false;
        }

        int width, height;
        glfwGetWindowSize(input->windowPtr, &width, &height);
        updateCameraProjectionMatrix(camera, kFov, static_cast<float>(width), static_cast<float>(height));

        TranslationComponent& translation{manager.getComponent<TranslationComponent>(entity)};
        if (input->mouseLock)
        {
            turnCameraFromInput(camera, translation, input->deltaMousePos);
        }

        // Update camera position
        if (InputSystem::isKeyPressedDown(*input, GLFW_KEY_W))
            moveForward(camera, translation, 0.01f);
        if (InputSystem::isKeyPressedDown(*input, GLFW_KEY_S))
            moveForward(camera, translation, -0.01f);
        if (InputSystem::isKeyPressedDown(*input, GLFW_KEY_D))
            moveRight(camera, translation, 0.01f);
        if (InputSystem::isKeyPressedDown(*input, GLFW_KEY_A))
            moveRight(camera, translation, -0.01f);
        return true;
    }};
    manager.forEachComponent<CameraComponent, TranslationComponent>(forEachCamera);
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
    float& yaw{translation.rotation.y};
    float& pitch{translation.rotation.x};

    // Change in input x axis changes yaw and input y axis changes pitch
    yaw += static_cast<float>(deltaInput.x) * kSensitivity;
    pitch += static_cast<float>(deltaInput.y) * kSensitivity;

    yaw = fmod(yaw, 360.0f);

    // Prevent from moving camera past looking straight up or down
    pitch = std::min(pitch, kPitchMax);
    pitch = std::max(pitch, -1.0F * kPitchMax);

    camera.cameraFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    camera.cameraFront.y = sin(glm::radians(pitch));
    camera.cameraFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camera.cameraFront = glm::normalize(camera.cameraFront);
}

void CameraSystem::updateCameraProjectionMatrix(CameraComponent& camera, float const fov, float const width,
                                                float const height)
{
    camera.proj = glm::perspective(glm::radians(fov), width / height, 0.1f, 1000.0f);
}
