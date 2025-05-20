#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera();

    void processMouseMovement(float xoffset, float yoffset);
    void processMouseScroll(float yoffset);
    void updateViewMatrix();

    void setMousePressed(bool pressed);
    void resetMouse(float xpos, float ypos);
    glm::mat4 getViewMatrix() const;
    glm::vec3 getPosition() const;

private:
    float yaw, pitch;
    float distance;  // Distance from center
    float lastX, lastY;
    bool firstMouse;
    bool leftMousePressed;

    glm::vec3 position;
    glm::vec3 center;  // Center point to look at (table center)
    glm::mat4 viewMatrix;
};