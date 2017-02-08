#pragma once
#ifndef GAMEPAD_H
#define GAMEPAD_H

#include <windows.h>
#include <Xinput.h>
#include <GLFW/glfw3.h>

// XInput Button values
static const WORD XINPUT_Buttons[] = {
	XINPUT_GAMEPAD_A,
	XINPUT_GAMEPAD_B,
	XINPUT_GAMEPAD_X,
	XINPUT_GAMEPAD_Y,
	XINPUT_GAMEPAD_DPAD_UP,
	XINPUT_GAMEPAD_DPAD_DOWN,
	XINPUT_GAMEPAD_DPAD_LEFT,
	XINPUT_GAMEPAD_DPAD_RIGHT,
	XINPUT_GAMEPAD_LEFT_SHOULDER,
	XINPUT_GAMEPAD_RIGHT_SHOULDER,
	XINPUT_GAMEPAD_LEFT_THUMB,
	XINPUT_GAMEPAD_RIGHT_THUMB,
	XINPUT_GAMEPAD_START,
	XINPUT_GAMEPAD_BACK
};

// XInput Button IDs
struct XButtonIDs
{

	XButtonIDs(); // Default constructor

	int A, B, X, Y; // 'Action' buttons

					// Directional Pad (D-Pad)
	int DPad_Up, DPad_Down, DPad_Left, DPad_Right;

	// Shoulder ('Bumper') buttons
	int L_Shoulder, R_Shoulder;

	// Thumbstick buttons
	int L_Thumbstick, R_Thumbstick;

	int Start; // 'START' button
	int Back;  // 'BACK' button
};

class Gamepad
{
public:
	Gamepad();
	Gamepad(int a_iIndex);
	~Gamepad();

	void Update();
	void RefreshState();

	//ThumbStick functions
	//Return true if the stick is inside a deadzone
	bool LStick_Dead();
	bool RStick_Dead();

	//Returns axis of corresponding stick in desired axis
	float LeftStick_X();
	float LeftStick_Y();
	float RightStick_X();
	float RightStick_Y();

	// Trigger functions
	float LeftTrigger();  // Return value of left trigger
	float RightTrigger(); // Return value of right trigger

	//Button Functions 
	//Button is pressed return true
	bool GetButtonPressed(int a_iButton);
	bool GetButtonDown(int a_iButton);

	//Utility functions created
	XINPUT_STATE GetState(); // Return gamepad state
	int GetIndex();          // Return gamepad index
	bool Connected();        // Return true if gamepad is connected

	//Vibration function should be (0.0f cancel, 1.0 max speed)
	void Rumble(float a_fLeftMotor, float a_fRightMotor);

	//glfw joystick input because current xinput tests are not working
	void glfwJoystick();

private:
	//member variables
	XINPUT_STATE m_State;	//current gamepad state
	int m_iGamepadIndex;	//index of controllers 1-4

	static const int ButtonCount = 14; //amount of buttons
	bool bPrev_ButtonsStates[ButtonCount]; //previous button frame
	bool bButtonStates[ButtonCount]; // current button frame

	//button pressed on current frame
	bool bGamepad_ButtonsDown[ButtonCount];
};

#endif // GAMEPAD_H

//Externally define the XbuttonIDs struct as XButtons
extern XButtonIDs Xbuttons;
