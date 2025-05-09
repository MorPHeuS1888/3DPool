#include "Camera.h"

Camera::Camera()
    : yaw(-90.0f), pitch(20.0f), radius(5.0f),
    lastX(0.0f), lastY(0.0f), firstMouse(true),
    leftMousePressed(false), position(0.0f) {
    updateViewMatrix();
}

void Camera::processMouseMovement(float xpos, float ypos) {
    if (!leftMousePressed) return;

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    yaw += xoffset * sensitivity;
    pitch += yoffset * sensitivity;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    updateViewMatrix();
}

void Camera::processMouseScroll(float yoffset) {
    radius -= yoffset * 0.5f;
    if (radius < 1.0f) radius = 1.0f;
    if (radius > 20.0f) radius = 20.0f;

    updateViewMatrix();
}

void Camera::updateViewMatrix() {
    float camX = radius * cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    float camY = radius * sin(glm::radians(pitch));
    float camZ = radius * cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    position = glm::vec3(camX, camY, camZ);

    viewMatrix = glm::lookAt(position, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 Camera::getViewMatrix() const {
    return viewMatrix;
}

glm::vec3 Camera::getPosition() const {
    return position;
}

void Camera::setMousePressed(bool pressed) {
    leftMousePressed = pressed;
    if (!pressed) firstMouse = true;
}

void Camera::resetMouse(float xpos, float ypos) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
}
