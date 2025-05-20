#include "Camera.h"
#include <iostream> // Add this include for debug output

Camera::Camera()
    : yaw(-90.0f), pitch(30.0f), distance(5.0f),
    lastX(0.0f), lastY(0.0f), firstMouse(true),
    leftMousePressed(false), position(0.0f), center(0.0f, 0.5f, 0.0f) {
    updateViewMatrix();
}

void Camera::processMouseMovement(float xpos, float ypos) {

    if (firstMouse) {

        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.3f;
    yaw += xoffset * sensitivity;
    pitch += yoffset * sensitivity;

    // Constrain pitch to avoid flipping
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
    updateViewMatrix();
}

void Camera::processMouseScroll(float yoffset) {
    distance -= yoffset * 0.2f;
    if (distance < 1.0f) distance = 1.0f;
    if (distance > 15.0f) distance = 15.0f;
    updateViewMatrix();
}

void Camera::updateViewMatrix() {
    // Calculate new camera position
    position.x = center.x + distance * cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    position.y = center.y + distance * sin(glm::radians(pitch));
    position.z = center.z + distance * sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    viewMatrix = glm::lookAt(position, center, glm::vec3(0.0f, 1.0f, 0.0f));
    
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