#define GLEW_STATIC
#pragma once
#include <GL/glew.h>
#include "Renderable.h"

class PoolTable : public Renderable {
public:
    PoolTable();    // Construtor
    ~PoolTable();   // Destruidor

    void setup();    // Configuração do paralelepípedo (mesa de bilhar)
    void render() override;   // Renderização da mesa de bilhar
    GLuint getShaderProgram() const;

private:
    GLuint VBO, VAO;// Buffers do OpenGL
    GLuint shaderProgram;
};
