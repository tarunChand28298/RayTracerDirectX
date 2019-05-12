#pragma once
#include <Windows.h>
#include <Xinput.h>

struct Keyboard 
{
	bool buttonDown[256];
	bool kbdStateChanged[256];
	BYTE prevKbdState[256];
	BYTE currKbdState[256];
	BYTE defKbdState[256];

	void StartKeyboard() {
		memset(prevKbdState, 0, sizeof(BYTE)*ARRAYSIZE(prevKbdState));
		memset(buttonDown, false, sizeof(bool)*ARRAYSIZE(buttonDown));
	}

	void UpdateKeyboard() {
		memcpy(prevKbdState, currKbdState, sizeof(BYTE)*ARRAYSIZE(prevKbdState));
		GetKeyboardState(currKbdState);
		SetKeyboardState(defKbdState);	//TODO: If there is problem with caps-lock and stuff, this is probably the cause.
		
		for (int i = 0; i < 256; i++)
		{
			if (prevKbdState[i] ^ currKbdState[i]) { kbdStateChanged[i] = true; }
			else { kbdStateChanged[i] = false; }

			if (kbdStateChanged[i]) { buttonDown[i] = !buttonDown[i]; }
		}
	}
};

struct Mouse 
{

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
