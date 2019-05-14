#pragma once
#include <Windows.h>
#include <Xinput.h>

struct Keyboard 
{
	bool spaceDown;
	bool shiftDown;

	float horizontal;
	float vertical;

	void UpdateKeyboard() {
		spaceDown = GetAsyncKeyState(VK_SPACE);
		shiftDown = GetAsyncKeyState(VK_SHIFT);

		horizontal = 0;
		if (GetAsyncKeyState('D')) { horizontal += 1; }
		if (GetAsyncKeyState('A')) { horizontal -= 1; }

		vertical = 0;
		if (GetAsyncKeyState('W')) { vertical += 1; }
		if (GetAsyncKeyState('S')) { vertical -= 1; }
	}
};

struct Mouse 
{
	HWND relativeToWindow;
	POINT mousePos;
	bool leftMouseButton;
	bool rightMouseButton;

	void UpdateMouse() {
		GetCursorPos(&mousePos);
		ScreenToClient(relativeToWindow, &mousePos);

		leftMouseButton = GetKeyState(VK_LBUTTON) & 0x80;
		rightMouseButton = GetKeyState(VK_RBUTTON) & 0x80;
	}
};

struct XBoxController
{
	bool start;
	bool back;
	bool Abutton;
	bool Bbutton;
	bool Xbutton;
	bool Ybutton;
	float leftStickX;
	float leftStickY;
	float rightStickX;
	float rightStickY;

	void UpdateController() {
		XINPUT_STATE controllerState;
		XInputGetState(0, &controllerState);

		start = controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_START;
		back = controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK;
		Abutton = controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_A;
		Bbutton = controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_B;
		Xbutton = controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_X;
		Ybutton = controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_Y;

		short leftStickXRaw = controllerState.Gamepad.sThumbLX;
		short leftStickYRaw = controllerState.Gamepad.sThumbLY;
		short rightStickXRaw = controllerState.Gamepad.sThumbRX;
		short rightStickYRaw = controllerState.Gamepad.sThumbRY;

		leftStickX = float(leftStickXRaw) / float(32768);
		leftStickY = float(leftStickYRaw) / float(32768);
		rightStickX = float(rightStickXRaw) / float(32768);
		rightStickY = float(rightStickYRaw) / float(32768);

	}
};
