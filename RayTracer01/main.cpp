#include "RenderingSystem/RenderingSystem.h"
#include "SceneSystem/Scene.h"
#include <string>

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, char* cmdArgs, int cmdShow) {
	
	RenderingWindow mainWindow(instance);
	Scene mainScene;

	RenderingSystem renderer(mainScene, mainWindow);

	while (mainWindow.running) {
		mainWindow.HandleMessages();
		mainScene.Update();
		renderer.Render();
	}

	return 0;
}
