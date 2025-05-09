#pragma once
#ifndef SHADER_UTILS_H
#define SHADER_UTILS_H

#include <string>
#include <GL/glew.h>

// Lê um shader de um ficheiro
std::string readShaderSource(const std::string& filePath);

// Compila um shader individual
GLuint compileShader(GLenum type, const char* source);

// Compila e linka um programa completo
GLuint createShaderProgram(const char* vertexSource, const char* fragmentSource);

#endif

