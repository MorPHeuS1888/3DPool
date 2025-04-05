#include "Window.h"
#include "PoolTable.h"

int main() {
    Window mainWindow(800, 600, "Janela OpenGL");
	PoolTable poolTable;
	poolTable.setup();

    while (!mainWindow.shouldClose()) {
        mainWindow.processInput();
        mainWindow.update(&poolTable);
    }

    return 0;
}