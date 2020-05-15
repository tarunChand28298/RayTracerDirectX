#include "RenderingSystem/RenderingSystem.h"
#include "SceneSystem/Scene.h"
//This is Ray tracing main file which is used for rendering
int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, char* cmdArgs, int cmdShow) {
	
	RenderingWindow mainWindow(instance);
	Scene mainScene; mainScene.mouse.relativeToWindow = mainWindow.windowHandle;
	RenderingSystem renderer(mainScene, mainWindow);

	while (mainWindow.running) {
		mainWindow.HandleMessages();
		mainScene.Update();
		renderer.Render();
	}

	return 0;
}
