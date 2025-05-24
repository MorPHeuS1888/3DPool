#include "Camera.h"

Camera::Camera()
    : position(0.0f, 0.0f, 1.0f),
    target(0.0f, 0.0f, -1.0f) {
    viewMatrix = glm::lookAt(position, target, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 Camera::getViewMatrix() const {
    return viewMatrix;
}

glm::vec3 Camera::getPosition() const {
    return position;
}