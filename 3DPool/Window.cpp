#define GLEW_STATIC
#include "Window.h"
#include "PoolTable.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include "Balls.h"

Window::Window(int width, int height, const char* title) : width(width), height(height) {
    if (!glfwInit()) {
        std::cerr << "Falha ao inicializar o GLFW" << std::endl;
        exit(-1);
    }

    // Configura versão do OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);

    // Cria a janela
    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        std::cerr << "Falha ao criar a janela GLFW" << std::endl;
        glfwTerminate();
        exit(-1);
    }


    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);

    // Configura callbacks
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback); // Novo callback

    // Inicializa o GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Falha ao inicializar o GLEW" << std::endl;
        exit(-1);
    }

    // Configurações OpenGL
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

// Callback de redimensionamento (novo)
void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    self->width = width;
    self->height = height;
    glViewport(0, 0, width, height);
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(window); // Retorna o status de fechamento da janela
}

void Window::processInput() {
    glfwPollEvents();
}

void Window::update(Renderable* scene) {
    // Atualiza matriz de projeção com novo aspect ratio
    glm::mat4 projection = glm::perspective(
        glm::radians(70.0f),
        static_cast<float>(width) / static_cast<float>(height),
        0.1f,
        20.0f
    );

    glm::mat4 view = camera.getViewMatrix();
    glm::vec3 lightPos(1.0f, 2.0f, 2.0f);
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

    // Renderização principal
    glViewport(0, 0, width, height);
    if (scene) {
        scene->render(view, projection, lightPos, camera.getPosition(), lightColor, true);
    }

    // Mini mapa (ajuste dinâmico)
    int miniMapSize = 200;
    int margin = 10;
    glViewport(
        width - miniMapSize - margin,
        height - miniMapSize - margin,
        miniMapSize,
        miniMapSize
    );

    glm::mat4 miniMapProjection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, 0.1f, 10.0f);
    glm::mat4 miniMapView = glm::lookAt(
        glm::vec3(0.0f, 5.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, -1.0f)
    );

    if (scene) {
        scene->render(miniMapView, miniMapProjection, lightPos, camera.getPosition(), lightColor, false);
    }

    // Restaura viewport principal
    glViewport(0, 0, width, height);
}

// Outros callbacks mantidos iguais
void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void Window::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    self->camera.processMouseMovement(static_cast<float>(xpos), static_cast<float>(-ypos));
}

void Window::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        self->camera.setMousePressed(action == GLFW_PRESS);
    }
}

void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    self->camera.processMouseScroll(static_cast<float>(yoffset));
}

GLFWwindow* Window::getWindow() const {
    return window; // Retorna o ponteiro da janela GLFW
}