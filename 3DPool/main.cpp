
#include "Window.h"
#include "PoolTable.h"
#include "Balls.h"

int main() {
    Window mainWindow(800, 600, "Janela OpenGL");

    double lastTime = glfwGetTime();

    while (!mainWindow.shouldClose()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        double currentTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentTime - lastTime);
        lastTime = currentTime;

        mainWindow.processInput();
        mainWindow.updateSceneContext(deltaTime); // Atualiza e renderiza todas as cenas

        glfwSwapBuffers(mainWindow.getWindow());
        glfwPollEvents();
    }
    return 0;
}

