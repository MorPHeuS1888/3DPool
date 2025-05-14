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

        // Limpa os buffers uma vez por frame
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Renderiza a mesa primeiro
        mainWindow.update(&poolTable);

        // Renderiza as bolas depois
        mainWindow.update(&balls);

        // Troca os buffers uma vez por frame
        glfwSwapBuffers(mainWindow.getWindow());
        glfwPollEvents();
    }
    return 0;
}