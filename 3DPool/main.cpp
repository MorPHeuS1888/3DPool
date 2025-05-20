
#include "Window.h"
#include "PoolTable.h"
#include "Balls.h"

int main() {
    Window mainWindow(800, 600, "Janela OpenGL");
    PoolTable poolTable;
    Balls balls;

    poolTable.setup();
    balls.setup();

    while (!mainWindow.shouldClose()) {
        mainWindow.processInput();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mainWindow.update(&poolTable);
        mainWindow.update(&balls);

        glfwSwapBuffers(mainWindow.getWindow());
        glfwPollEvents();
    }
    return 0;
}