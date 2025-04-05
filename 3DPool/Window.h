#pragma once 

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Renderable.h"
#include <iostream>

class Window {
public:
    Window(int width, int height, const char* title);
    ~Window();

    void processInput(); // Processa eventos de teclado
    void update(Renderable* scene); // Troca buffers e processa eventos
    bool shouldClose() const; // Verifica se a janela deve fechar

private:
    GLFWwindow* window; // Ponteiro para a janela GLFW
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    int width, height; // Largura e altura da janela
};
