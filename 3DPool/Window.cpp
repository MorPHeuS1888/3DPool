#include "Window.h"
#include "Renderable.h" 
#include "PoolTable.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
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

    // Cria matrizes com GLM
    glm::mat4 projection = glm::perspective(glm::radians(70.0f), (float)width / height, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 3.0f, 5.0f),  // posição da câmera
        glm::vec3(0.0f, 0.0f, 0.0f),  // para onde olha
        glm::vec3(0.0f, 1.0f, 0.0f)   // vetor up
    );

    // Passa as matrizes para o shader do objeto (se possível)
    PoolTable* table = dynamic_cast<PoolTable*>(scene);
    if (table) {
        GLuint shader = table->getShaderProgram();  
        glUseProgram(shader);

        GLint viewLoc = glGetUniformLocation(shader, "view");
        GLint projLoc = glGetUniformLocation(shader, "projection");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    }

    // Renderiza a cena
    if (scene) scene->render();

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