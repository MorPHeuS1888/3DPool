#include "Window.h"
#include "Renderable.h" 
#include <GL/glu.h>

Window::Window(int width, int height, const char* title) : width(width), height(height) {
    if (!glfwInit()) {
        std::cerr << "Falha ao inicializar o GLFW" << std::endl;
        exit(-1);
    }

    // Configura versão do OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Cria a janela
    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        std::cerr << "Falha ao criar a janela GLFW" << std::endl;
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback); // Define o callback de teclado

    // Inicializa o GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Falha ao inicializar o GLEW" << std::endl;
        exit(-1);
    }

	glViewport(0, 0, width, height);

	glEnable(GL_DEPTH_TEST); // Habilita o teste de profundidade

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f); // Cor de fundo da janela
}

Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::processInput() {
    glfwPollEvents();
}

void Window::update(Renderable* scene) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70.0, (float)width / height, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 3, 5, 0, 0, 0, 0, 1, 0);

    if (scene) scene->render(); // desenha sem saber o que é

    glfwSwapBuffers(window);
    glfwPollEvents();
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(window);
}

// Callback de entrada do teclado
void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}