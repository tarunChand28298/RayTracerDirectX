#pragma once
#include <Windows.h>

class RenderingWindow
{
public:
	RenderingWindow(HINSTANCE instance);
	~RenderingWindow();

	HWND windowHandle;
	bool running;
	int width;
	int height;

	void Initialize(HINSTANCE instance);
	void HandleMessages();
	void Destroy();

	static LRESULT CALLBACK WindowCallback(HWND windowHandle, UINT message, WPARAM wparam, LPARAM lparam);
};

