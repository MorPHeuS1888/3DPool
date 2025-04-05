#include "PoolTable.h"
#include <GL/glew.h>
#include <iostream>

PoolTable::PoolTable() : VBO(0), VAO(0) {}

PoolTable::~PoolTable() {
    // Limpeza dos recursos OpenGL
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void PoolTable::setup() {
    GLfloat vertices[] = {
        -1.0f, -0.5f, -1.5f,  1.0f, 0.0f, 0.0f,
         1.0f, -0.5f, -1.5f,  1.0f, 0.0f, 0.0f,
         1.0f,  0.5f, -1.5f,  1.0f, 0.0f, 0.0f,

        -1.0f, -0.5f,  1.5f,  1.0f, 0.0f, 1.0f,
         1.0f, -0.5f,  1.5f,  0.0f, 1.0f, 1.0f,
         1.0f,  0.5f,  1.5f,  0.5f, 0.5f, 0.5f,
         // Outras faces da mesa aqui
    };

    // Criando VAO e VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Layout dos atributos: posição e cor
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void PoolTable::render() {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6); // Ajuste o número de vértices conforme necessário
    glBindVertexArray(0);
}
