#define GLEW_STATIC
#include "PoolTable.h"
#include "ShaderUtils.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>


PoolTable::PoolTable() {}

PoolTable::~PoolTable() { }

void PoolTable::setup() {
    ShaderInfo shaders[] = {
        { GL_VERTEX_SHADER, "shaders/vertexShader.ver" },
        { GL_FRAGMENT_SHADER, "shaders/fragmentShader.frag" },
        { GL_NONE, NULL }
    };

    shaderProgram = LoadShaders(shaders);

    // Define vertices for the pool table
    std::vector<float> vertices = {
        // Posições           // Normais           // TexCoords
        // Face frontal
        -0.71f, -0.39f,  1.42f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
         0.71f, -0.39f,  1.42f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
         0.71f,  0.00f,  1.42f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
        -0.71f, -0.39f,  1.42f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
         0.71f,  0.00f,  1.42f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
        -0.71f,  0.00f,  1.42f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,

        // Face traseira
        -0.71f, -0.39f, -1.42f,  0.0f, 0.0f, -1.0f,  1.0f, 0.0f,
         0.71f,  0.00f, -1.42f,  0.0f, 0.0f, -1.0f,  0.0f, 1.0f,
         0.71f, -0.39f, -1.42f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f,
        -0.71f, -0.39f, -1.42f,  0.0f, 0.0f, -1.0f,  1.0f, 0.0f,
        -0.71f,  0.00f, -1.42f,  0.0f, 0.0f, -1.0f,  1.0f, 1.0f,
         0.71f,  0.00f, -1.42f,  0.0f, 0.0f, -1.0f,  0.0f, 1.0f,

         // Face esquerda
         -0.71f, -0.39f, -1.42f, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
         -0.71f, -0.39f,  1.42f, -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
         -0.71f,  0.00f,  1.42f, -1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
         -0.71f, -0.39f, -1.42f, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
         -0.71f,  0.00f,  1.42f, -1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
         -0.71f,  0.00f, -1.42f, -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,

         // Face direita
          0.71f, -0.39f, -1.42f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
          0.71f,  0.00f,  1.42f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
          0.71f, -0.39f,  1.42f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
          0.71f, -0.39f, -1.42f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
          0.71f,  0.00f, -1.42f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
          0.71f,  0.00f,  1.42f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,

          // Face superior
          -0.71f,  0.00f, -1.42f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
          -0.71f,  0.00f,  1.42f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
           0.71f,  0.00f,  1.42f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
          -0.71f,  0.00f, -1.42f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
           0.71f,  0.00f,  1.42f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
           0.71f,  0.00f, -1.42f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,

           // Face inferior
           -0.71f, -0.39f, -1.42f,  0.0f, -1.0f, 0.0f,  1.0f, 1.0f,
            0.71f, -0.39f,  1.42f,  0.0f, -1.0f, 0.0f,  0.0f, 0.0f,
           -0.71f, -0.39f,  1.42f,  0.0f, -1.0f, 0.0f,  1.0f, 0.0f,
           -0.71f, -0.39f, -1.42f,  0.0f, -1.0f, 0.0f,  1.0f, 1.0f,
            0.71f, -0.39f, -1.42f,  0.0f, -1.0f, 0.0f,  0.0f, 1.0f,
            0.71f, -0.39f,  1.42f,  0.0f, -1.0f, 0.0f,  0.0f, 0.0f
    };

    // visto que cada v��rtice tem 8 elementos (posi����o, normal e coordenadas de textura)
    model.GetModelData().vertices = vertices;
    model.GetModelData().vertexCount = vertices.size() / 8; // 8 elements per vertex
    model.Install();
    model.BindShaderAttributes(shaderProgram);

    glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 0);
}

void PoolTable::applySceneContext(const glm::mat4& view, const glm::mat4& projection,
    const glm::vec3& lightPos, const glm::vec3& viewPos,
    const glm::vec3& lightColor, bool useLighting) {
    glUseProgram(shaderProgram);

    // Envia as matrizes para o shader
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));

    // Envia as variáveis de luz
    glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, glm::value_ptr(lightPos));
    glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, glm::value_ptr(viewPos));
    glUniform3fv(glGetUniformLocation(shaderProgram, "lightColor"), 1, glm::value_ptr(lightColor));

    // Desativa a textura
    glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 0);
    glUniform3f(glGetUniformLocation(shaderProgram, "objectColor"), 0.0f, 0.6f, 0.0f);

    // Renderiza a mesa de bilhar
    model.Render(glm::vec3(0.0f), glm::vec3(0.0f));
}

