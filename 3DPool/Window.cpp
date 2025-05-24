#define GLEW_STATIC
#include "Window.h"
#include "PoolTable.h"
#include "light.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include "Balls.h"
#include "Animation.h"


Window::Window(int width, int height, const char* title) : width(width), height(height), rotationX(0.0f), rotationY(0.0f), zoom(-3.0f), isMouseDragging(false), lastMouseX(0), lastMouseY(0), animation(balls, poolTable) {
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

    background.Load();
    poolTable.setup();
    balls.setup();
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



void Window::update(PoolLibrary::Renderable* scene, float deltaTime) {
    animation.Update(deltaTime);

    // Atualiza matriz de projeção
    glm::mat4 projection = glm::perspective(
        glm::radians(70.0f),
        static_cast<float>(width) / static_cast<float>(height),
        0.1f,
        20.0f
    );

    // Matrizes de transformação global (para a cena, não para o skybox)
    glm::mat4 globalModel = glm::mat4(1.0f);
    globalModel = glm::translate(globalModel, glm::vec3(0.0f, 0.0f, zoom));
    globalModel = glm::rotate(globalModel, rotationX, glm::vec3(1.0f, 0.0f, 0.0f));
    globalModel = glm::rotate(globalModel, rotationY, glm::vec3(0.0f, 1.0f, 0.0f));

    // View matrix da câmera (sem transformações globais)
    glm::mat4 view = camera.getViewMatrix();
    glm::vec3 lightPos(1.0f, 2.0f, 2.0f);
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

    // 1. Primeiro renderiza o skybox (sem transformações globais)
    glDepthMask(GL_FALSE); // Desativa escrita no depth buffer
    background.applySceneContext(view, projection, lightPos, camera.getPosition(), lightColor, true);
    glDepthMask(GL_TRUE); // Reativa para outros objetos

    // 2. Depois renderiza a cena com transformações globais
    glUseProgram(scene->getShaderProgram());
    glUniformMatrix4fv(
        glGetUniformLocation(scene->getShaderProgram(), "globalModel"),
        1, GL_FALSE, glm::value_ptr(globalModel)
    );

    light.update(scene->getShaderProgram(), camera.getPosition());

    // Renderização principal
    glViewport(0, 0, width, height);
    if (scene) {
        scene->applySceneContext(view, projection, lightPos, camera.getPosition(), lightColor, true);
    }


    // Mini mapa: Forçar apenas luz ambiente
    int miniMapSize = 200;
    int margin = 10;
    glViewport(
        width - miniMapSize - margin,
        height - miniMapSize - margin,
        miniMapSize,
        miniMapSize
    );

    glm::mat4 miniMapProjection = glm::ortho(-1.5f, 1.5f, -2.5f, 0.5f, 0.1f, 10.0f);
    glm::mat4 miniMapView = glm::lookAt(
        glm::vec3(0.0f, 5.0f, 0.0f), // Posição da câmera (acima da mesa)
        glm::vec3(0.0f, 0.0f, -1.0f), // Olhando para o centro da mesa
        glm::vec3(0.0f, 0.0f, -1.0f)  // Orientação corrigida (eixo Y para cima)
    );

    if (scene) {
        // Forçar luz ambiente e desativar outras
        GLuint shader = scene->getShaderProgram();
        glUseProgram(shader);
        glUniform1i(glGetUniformLocation(shader, "ambientEnabled"), 1);
        glUniform1i(glGetUniformLocation(shader, "directionalEnabled"), 0);
        glUniform1i(glGetUniformLocation(shader, "pointEnabled"), 0);
        glUniform1i(glGetUniformLocation(shader, "spotEnabled"), 0);
        glUniform3f(glGetUniformLocation(shader, "ambientLight"), 1.0f, 1.0f, 1.0f);

        // Renderize sem aplicar transformações globais (globalModel)
        glUniformMatrix4fv(
            glGetUniformLocation(shader, "globalModel"),
            1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)) // Matriz identidade
        );

        scene->applySceneContext(miniMapView, miniMapProjection, lightPos, camera.getPosition(), lightColor, true);
    }

    // Restaura viewport principal
    glViewport(0, 0, width, height);
}

void Window::updateSceneContext(float deltaTime) {
    animation.Update(deltaTime);



    update(&background, deltaTime);


    // 2. Atualizar e renderizar a mesa
    update(&poolTable, deltaTime);

    // 3. Atualizar e renderizar as bolas
    update(&balls, deltaTime);
}


// Outros callbacks mantidos iguais
void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (action == GLFW_PRESS) {
        std::cout << "Key PRESSED: ";
        switch (key) {
        case GLFW_KEY_1:
            self->light.toggleAmbient();
            std::cout << "1 (Ambient light) - Estado: "
                << (self->light.isAmbientEnabled() ? "ON" : "OFF");
            break;
        case GLFW_KEY_2:
            self->light.toggleDirectional();
            std::cout << "2 (Directional light) - Estado: "
                << (self->light.isDirectionalEnabled() ? "ON" : "OFF");
            break;
        case GLFW_KEY_3:
            self->light.togglePoint();
            std::cout << "3 (Point light) - Estado: "
                << (self->light.isPointEnabled() ? "ON" : "OFF");
            break;
        case GLFW_KEY_4:
            self->light.toggleSpot();
            std::cout << "4 (Spot light) - Estado: "
                << (self->light.isSpotEnabled() ? "ON" : "OFF");
            break;
        case GLFW_KEY_ESCAPE:
            std::cout << "ESC key pressed - closing window" << std::endl;
            glfwSetWindowShouldClose(window, true);
            break;
        case GLFW_KEY_SPACE:
            if (!self->animation.IsAnimating()) {
                std::cout << "\n--- STARTING NEW ANIMATION ---\n";
                self->animation.StartRandomAnimation();
            }
            else {
                std::cout << "Animation already in progress!\n";
            }
            break;
        default:
            std::cout << "Unknown key: " << key;
            break;
        }
        std::cout << std::endl;
    }
    else if (action == GLFW_REPEAT) {
        // Mantido igual pois o repeat não altera estados
        std::cout << "Key REPEAT: ";
        switch (key) {
        case GLFW_KEY_1: std::cout << "1"; break;
        case GLFW_KEY_2: std::cout << "2"; break;
        case GLFW_KEY_3: std::cout << "3"; break;
        case GLFW_KEY_4: std::cout << "4"; break;
        default: std::cout << key; break;
        }
        std::cout << std::endl;
    }
}

void Window::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (self->isMouseDragging) {
        self->rotationY += (xpos - self->lastMouseX) * 0.01f;
        self->rotationX += (self->lastMouseY - ypos) * 0.01f; // Invertido para movimento natural
        self->lastMouseX = xpos;
        self->lastMouseY = ypos;
    }
}

void Window::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        self->isMouseDragging = (action == GLFW_PRESS);
        glfwGetCursorPos(window, &self->lastMouseX, &self->lastMouseY);
    }
}

void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    self->zoom += static_cast<float>(yoffset) * 0.2f;
    self->zoom = glm::clamp(self->zoom, -10.0f, -1.0f);
}

GLFWwindow* Window::getWindow() const {
    return window; // Retorna o ponteiro da janela GLFW
}