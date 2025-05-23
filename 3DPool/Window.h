#pragma once 

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "Light.h"
#include <iostream>
#include "ModelManager.h"
#include "Animation.h"
#include "PoolTable.h"
#include "Balls.h"

class Window {
public:
    Window(int width, int height, const char* title);
    ~Window();

    void processInput();
    void update(PoolLibrary::Renderable* scene, float deltaTime);
    void updateSceneContext(float deltaTime);
    bool shouldClose() const;
    GLFWwindow* getWindow() const;

private:
    GLFWwindow* window;
    int width, height;
    Light light;
    PoolTable poolTable;  // Declarado primeiro
    Balls balls;          // Declarado depois
    Animation animation;  // Depende dos anteriores
    float rotationX, rotationY;
    float zoom;
    bool isMouseDragging;
    double lastMouseX, lastMouseY;

    // Callbacks estáticos
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height); // Adicione esta linha
   

    Camera camera;
};