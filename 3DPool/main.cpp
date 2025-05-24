
#include "Window.h"
#include "PoolTable.h"
#include "Balls.h"

int main() {
    Window mainWindow(800, 600, "Janela OpenGL");

    double lastTime = glfwGetTime();

    while (!mainWindow.shouldClose()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        double currentTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentTime - lastTime); // tempo desde o último frame
        lastTime = currentTime; 

        mainWindow.processInput();
        mainWindow.updateSceneContext(deltaTime); // isto renderiza tudo por ordem 

        glfwSwapBuffers(mainWindow.getWindow());
        glfwPollEvents();
    }
    return 0;
}

