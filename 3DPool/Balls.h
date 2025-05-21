#pragma once
#include "Renderable.h"
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "light.h"
#include <string> 
#include "ModelManager.h" // Adicionar este include para ModelManager


class Balls : public Renderable {
public:
    Balls();
    ~Balls();

    void setup();
    void render(const glm::mat4& view, const glm::mat4& projection,
        const glm::vec3& lightPos, const glm::vec3& viewPos,
        const glm::vec3& lightColor, bool useLighting) override;
    GLuint getShaderProgram() const override { return shaderProgram; }

private:
    GLuint shaderProgram;
    struct Ball {
        PoolLibrary::ModelManager model;
        glm::vec3 position;
        glm::vec3 rotation; // Adicionado para armazenar rotação
    };

    std::vector<Ball> balls;

};