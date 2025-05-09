#define GLEW_STATIC
#include "Window.h"

#include "PoolTable.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>




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
    glfwSetKeyCallback(window, keyCallback);  // Define o callback de teclado

    // Registra os callbacks de mouse
    glfwSetWindowUserPointer(window, this);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, scrollCallback);

    // Inicializa o GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Falha ao inicializar o GLEW" << std::endl;
        exit(-1);
    }

    glViewport(0, 0, width, height);

    glEnable(GL_DEPTH_TEST);  // Habilita o teste de profundidade

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);  // Cor de fundo da janela
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

    // --- Configuração da câmera principal ---
    glm::mat4 projection = glm::perspective(glm::radians(70.0f), (float)width / height, 0.1f, 100.0f);
    glm::mat4 view = camera.getViewMatrix();

    // --- Configuração da câmera do mini mapa ---
    glm::mat4 miniMapProjection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, 0.1f, 10.0f);
    glm::mat4 miniMapView = glm::lookAt(
        glm::vec3(0.0f, 5.0f, 0.0f),  // posição da câmera do mini mapa (acima do retângulo)
        glm::vec3(0.0f, 0.0f, 0.0f),  // para onde olha
        glm::vec3(0.0f, 0.0f, -1.0f)  // vetor up (invertido para visão de cima)
    );

    // Posição e cor da luz
    glm::vec3 lightPos(1.0f, 2.0f, 2.0f);
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

    // Renderiza a visão principal
    PoolTable* table = dynamic_cast<PoolTable*>(scene);
    if (table) {
        GLuint shader = table->getShaderProgram();
        glUseProgram(shader);

        // Envia as matrizes para o shader
        glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));

        // Envia as variáveis de luz
        glUniform3fv(glGetUniformLocation(shader, "lightPos"), 1, glm::value_ptr(lightPos));
        glUniform3fv(glGetUniformLocation(shader, "viewPos"), 1, glm::value_ptr(camera.getPosition()));
        glUniform3fv(glGetUniformLocation(shader, "lightColor"), 1, glm::value_ptr(lightColor));

        // Ativa a iluminação
        glUniform1i(glGetUniformLocation(shader, "useLighting"), 1);
    }

    if (scene) scene->render();

   

    // Renderiza o mini mapa
    int miniMapWidth = 200;  // Largura do mini mapa
    int miniMapHeight = 200; // Altura do mini mapa
    glViewport(width - 150, height - 180, miniMapWidth, miniMapHeight);  // Define o viewport no canto superior direito

    if (table) {
        GLuint shader = table->getShaderProgram();
        glUseProgram(shader);

        // Envia as matrizes para o shader
        glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, glm::value_ptr(miniMapView));
        glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(miniMapProjection));
        glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));

        // Desativa a iluminação
        glUniform1i(glGetUniformLocation(shader, "useLighting"), 0);
    }

    if (scene) scene->render();

    // Restaura o viewport para a visão principal
    glViewport(0, 0, width, height);

    // Troca os buffers e processa eventos
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

// Callback de movimento do mouse (para a câmera orbital)
void Window::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    self->camera.processMouseMovement(static_cast<float>(xpos), static_cast<float>(-ypos));
}

// Callback de pressionamento do mouse (para ativar ou desativar o movimento)
void Window::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        self->camera.setMousePressed(action == GLFW_PRESS);
    }
}

// Callback de rolagem do mouse (para zoom)
void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    self->camera.processMouseScroll(static_cast<float>(yoffset));
}
