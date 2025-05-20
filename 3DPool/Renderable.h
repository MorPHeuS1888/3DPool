#pragma once
#include <GL/glew.h> // Adicionar este include
#include <glm/glm.hpp>

class Renderable {
public:
    virtual ~Renderable() = default;
    virtual GLuint getShaderProgram() const = 0; // Mantenha 'const' aqui
    virtual void render(const glm::mat4& view, const glm::mat4& projection,
        const glm::vec3& lightPos, const glm::vec3& viewPos,
        const glm::vec3& lightColor, bool useLighting) = 0;
};