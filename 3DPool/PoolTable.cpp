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


    std::string vertexCode = readShaderSource("shaders/vertexShader.glsl");
    std::string fragmentCode = readShaderSource("shaders/fragmentShader.glsl");

    shaderProgram = createShaderProgram(vertexCode.c_str(), fragmentCode.c_str());

    GLfloat vertices[] = {
        // --- Face frontal ---
        -0.71f, -0.39f,  1.42f,   0.0f, 0.6f, 0.0f,  // Cor verde
         0.71f, -0.39f,  1.42f,   0.0f, 0.6f, 0.0f,  // Cor verde
         0.71f,  0.00f,  1.42f,   0.0f, 0.6f, 0.0f,  // Cor verde

        -0.71f, -0.39f,  1.42f,   0.0f, 0.6f, 0.0f,  // Cor verde
         0.71f,  0.00f,  1.42f,   0.0f, 0.6f, 0.0f,  // Cor verde
        -0.71f,  0.00f,  1.42f,   0.0f, 0.6f, 0.0f,  // Cor verde

        // --- Face traseira ---
        -0.71f, -0.39f, -1.42f,   0.0f, 0.6f, 0.0f,  // Cor verde
         0.71f,  0.00f, -1.42f,   0.0f, 0.6f, 0.0f,  // Cor verde
         0.71f, -0.39f, -1.42f,   0.0f, 0.6f, 0.0f,  // Cor verde

        -0.71f, -0.39f, -1.42f,   0.0f, 0.6f, 0.0f,  // Cor verde
        -0.71f,  0.00f, -1.42f,   0.0f, 0.6f, 0.0f,  // Cor verde
         0.71f,  0.00f, -1.42f,   0.0f, 0.6f, 0.0f,  // Cor verde

         // --- Face esquerda ---
         -0.71f, -0.39f, -1.42f,   0.0f, 0.6f, 0.0f,  // Cor verde
         -0.71f, -0.39f,  1.42f,   0.0f, 0.6f, 0.0f,  // Cor verde
         -0.71f,  0.00f,  1.42f,   0.0f, 0.6f, 0.0f,  // Cor verde

         -0.71f, -0.39f, -1.42f,   0.0f, 0.6f, 0.0f,  // Cor verde
         -0.71f,  0.00f,  1.42f,   0.0f, 0.6f, 0.0f,  // Cor verde
         -0.71f,  0.00f, -1.42f,   0.0f, 0.6f, 0.0f,  // Cor verde

         // --- Face direita ---
          0.71f, -0.39f, -1.42f,   0.0f, 0.6f, 0.0f,  // Cor verde
          0.71f,  0.00f,  1.42f,   0.0f, 0.6f, 0.0f,  // Cor verde
          0.71f, -0.39f,  1.42f,   0.0f, 0.6f, 0.0f,  // Cor verde

          0.71f, -0.39f, -1.42f,   0.0f, 0.6f, 0.0f,  // Cor verde
          0.71f,  0.00f, -1.42f,   0.0f, 0.6f, 0.0f,  // Cor verde
          0.71f,  0.00f,  1.42f,   0.0f, 0.6f, 0.0f,  // Cor verde

          // --- Face superior ---
          -0.71f,  0.00f, -1.42f,   0.0f, 0.6f, 0.0f,  // Cor verde
          -0.71f,  0.00f,  1.42f,   0.0f, 0.6f, 0.0f,  // Cor verde
           0.71f,  0.00f,  1.42f,   0.0f, 0.6f, 0.0f,  // Cor verde

          -0.71f,  0.00f, -1.42f,   0.0f, 0.6f, 0.0f,  // Cor verde
           0.71f,  0.00f,  1.42f,   0.0f, 0.6f, 0.0f,  // Cor verde
           0.71f,  0.00f, -1.42f,   0.0f, 0.6f, 0.0f,  // Cor verde

           // --- Face inferior ---
           -0.71f, -0.39f, -1.42f,   0.0f, 0.6f, 0.0f,  // Cor verde
            0.71f, -0.39f,  1.42f,   0.0f, 0.6f, 0.0f,  // Cor verde
           -0.71f, -0.39f,  1.42f,   0.0f, 0.6f, 0.0f,  // Cor verde

           -0.71f, -0.39f, -1.42f,   0.0f, 0.6f, 0.0f,  // Cor verde
            0.71f, -0.39f, -1.42f,   0.0f, 0.6f, 0.0f,  // Cor verde
            0.71f, -0.39f,  1.42f,   0.0f, 0.6f, 0.0f,  // Cor verde
    };



    // Criando VAO e VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Layout dos atributos: posi��o e cor
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0); // posição
glEnableVertexAttribArray(0);

glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); // normal
glEnableVertexAttribArray(1);

glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); // cor
glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void PoolTable::render() {
    glUseProgram(shaderProgram);
    glUniform3fv(glGetUniformLocation(shaderProgram, "boardColor"), 1, glm::value_ptr(glm::vec3(0.0f, 0.6f, 0.0f)));
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}


// M�todo para fornecer o shader � janela (para setar view/projection)
GLuint PoolTable::getShaderProgram() const {
    return shaderProgram;
}
