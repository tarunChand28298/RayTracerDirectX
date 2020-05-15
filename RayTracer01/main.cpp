#include "RenderingSystem/RenderingSystem.h"
#include "SceneSystem/Scene.h"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, char* cmdArgs, int cmdShow) {
	
	RenderingWindow mainWindow(instance);
	Scene mainScene; mainScene.mouse.relativeToWindow = mainWindow.windowHandle;
	RenderingSystem renderer(mainScene, mainWindow);
	//update loop
	while (mainWindow.running) {
		mainWindow.HandleMessages();
		mainScene.Update();
		renderer.Render();
	}

	return 0;
}
