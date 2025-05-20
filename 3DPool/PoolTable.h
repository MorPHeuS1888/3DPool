// PoolTable.h
#define GLEW_STATIC
#pragma once
#include <GL/glew.h>
#include "Renderable.h"
#include "Balls.h" // This includes the PoolLibrary namespace

class PoolTable : public Renderable {
public:
    PoolTable();    // Construtor
    ~PoolTable();   // Destruidor

    void setup();    // Configuração do paralelepípedo (mesa de bilhar)
    void render(const glm::mat4& view, const glm::mat4& projection,
        const glm::vec3& lightPos, const glm::vec3& viewPos,
        const glm::vec3& lightColor, bool useLighting) override;
    GLuint getShaderProgram() const override { return shaderProgram; }

private:
    GLuint shaderProgram;
    PoolLibrary::ModelManager model;
};