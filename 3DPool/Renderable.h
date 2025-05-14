#pragma once

#pragma once
#include <glm/glm.hpp>

class Renderable {
public:
    virtual ~Renderable() = default;
    virtual void render(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& lightPos, const glm::vec3& viewPos, const glm::vec3& lightColor, bool useLighting) = 0;
};
