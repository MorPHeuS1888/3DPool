#define GLEW_STATIC
#include "PoolTable.h"
#include "ShaderUtils.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

PoolTable::PoolTable() : VBO(0), VAO(0) {}

PoolTable::~PoolTable() {
    // Limpeza dos recursos OpenGL
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void PoolTable::setup() {


    std::string vertexCode = readShaderSource("shaders/vertexShader.ver");
    std::string fragmentCode = readShaderSource("shaders/fragmentShader.frag");

    shaderProgram = createShaderProgram(vertexCode.c_str(), fragmentCode.c_str());

    GLfloat vertices[] = {
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

    glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 0);

    // Criando VAO e VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Layout dos atributos: posi��o e cor
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0); // Posição
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat))); // Normal
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat))); // TexCoord
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void PoolTable::render(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& lightPos, const glm::vec3& viewPos, const glm::vec3& lightColor, bool useLighting) {
    glUseProgram(shaderProgram);

    // Envia as matrizes para o shader
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));

    // Envia as variáveis de luz
    glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, glm::value_ptr(lightPos));
    glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, glm::value_ptr(viewPos));
    glUniform3fv(glGetUniformLocation(shaderProgram, "lightColor"), 1, glm::value_ptr(lightColor));

    // desativa a textura
    glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 0);
    glUniform3f(glGetUniformLocation(shaderProgram, "objectColor"), 0.0f, 0.6f, 0.0f);

    // Renderiza a mesa de bilhar
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}


// M�todo para fornecer o shader � janela (para setar view/projection)
GLuint PoolTable::getShaderProgram() const {
    return shaderProgram;
}
