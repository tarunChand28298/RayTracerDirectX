#include "RenderingWindow.h"

RenderingWindow::RenderingWindow(HINSTANCE instance)
{
	Initialize(instance);
}

RenderingWindow::~RenderingWindow()
{
	Destroy();
}

void RenderingWindow::Initialize(HINSTANCE instance)
{
	WNDCLASS wc = {};
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hInstance = instance;
	wc.lpfnWndProc = WindowCallback;
	wc.lpszClassName = "RenderingWindow";
	wc.style = CS_OWNDC;

	RegisterClass(&wc);
	windowHandle = CreateWindow("RenderingWindow", "Ray Tracer", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, instance, this);
	ShowWindow(windowHandle, SW_SHOW);

	running = true;
}

void RenderingWindow::HandleMessages()
{
	MSG message = {};
	while (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&message);
		DispatchMessage(&message);

		if (message.message == WM_CLOSE) {
			PostQuitMessage(0);
			running = false;
		}
	}
}

void RenderingWindow::Destroy()
{
	running = false;
}

LRESULT RenderingWindow::WindowCallback(HWND windowHandle, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message) {
		case WM_CREATE: {
			RenderingWindow* window = (RenderingWindow*)((LPCREATESTRUCT)lparam)->lpCreateParams;
			SetWindowLongPtr(windowHandle, GWLP_USERDATA, (LONG_PTR)window);
			break;
		}
		case WM_SIZE: {
			RenderingWindow* window = (RenderingWindow*)GetWindowLongPtr(windowHandle, GWLP_USERDATA);
			window->width = LOWORD(lparam);
			window->height = HIWORD(lparam);
			break;
		}
		case WM_QUIT: {
			RenderingWindow* window = (RenderingWindow*)GetWindowLongPtr(windowHandle, GWLP_USERDATA);
			DestroyWindow(window->windowHandle);
			break;
		}
		case WM_DESTROY: {
			RenderingWindow* window = (RenderingWindow*)GetWindowLongPtr(windowHandle, GWLP_USERDATA);
			window->Destroy();
			break;
		}
		default: {
			break;
		}
	}
	return DefWindowProc(windowHandle, message, wparam, lparam);
}
