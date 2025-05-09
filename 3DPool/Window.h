#pragma once 

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Renderable.h"
#include "Camera.h"
#include <iostream>

class Window {
public:
    Window(int width, int height, const char* title);
    ~Window();

    void processInput(); // Processa eventos de teclado
    void update(Renderable* scene); // Troca buffers e processa eventos
    bool shouldClose() const; // Verifica se a janela deve fechar

private:
    GLFWwindow* window;
    int width, height;

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    Camera camera;
};
