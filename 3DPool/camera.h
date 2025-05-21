#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera();

    glm::mat4 getViewMatrix() const;
    glm::vec3 getPosition() const;

private:
    glm::vec3 position;
    glm::vec3 target;
    glm::mat4 viewMatrix;
};