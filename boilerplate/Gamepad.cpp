#include "Gamepad.h"
#include <iostream>

using namespace std;

//Link to the Xinput library
#pragma comment(lib, "Xinput.lib")

XButtonIDs Xbuttons;
// 'XButtonIDs' - Default constructor
XButtonIDs::XButtonIDs()
{
	// These values are used to index the XINPUT_Buttons array,
	// accessing the matching XINPUT button value

	A = 0;
	B = 1;
	X = 2;
	Y = 3;

	DPad_Up = 4;
	DPad_Down = 5;
	DPad_Left = 6;
	DPad_Right = 7;

	L_Shoulder = 8;
	R_Shoulder = 9;

	L_Thumbstick = 10;
	R_Thumbstick = 11;

	Start = 12;
	Back = 13;
}

Gamepad::Gamepad()
{
}

// constructor for gamepad for joystick index
Gamepad::Gamepad(int a_iIndex)
{
	m_iGamepadIndex = a_iIndex - 1;

	for (int i = 0; i < ButtonCount; i++)
	{
		bPrev_ButtonsStates[i] = false;
		bButtonStates[i] = false;
		bGamepad_ButtonsDown[i] = false;
	}
}

Gamepad::~Gamepad()
{
}

XINPUT_STATE Gamepad::GetState()
{
	XINPUT_STATE GamepadState;

	ZeroMemory(&GamepadState, sizeof(XINPUT_STATE));

	XInputGetState(m_iGamepadIndex, &m_State);
	
	return GamepadState;
}

int Gamepad::GetIndex()
{
	cout << m_iGamepadIndex << endl;;
	return m_iGamepadIndex;
}

bool Gamepad::Connected()
{
	ZeroMemory(&m_State, sizeof(XINPUT_STATE));

	DWORD Result = XInputGetState(m_iGamepadIndex, &m_State);

	if (Result == ERROR_SUCCESS)
	{
		cout << "connected " << endl;
		return true;
	}
	else
	{
		cout << "not connected " << endl;
 		return false;
	}
}

void Gamepad::Update()
{
	m_State = GetState(); //Get current gamepad status

	for (int i = 0; i < ButtonCount; i++)
	{
		bButtonStates[i] = (m_State.Gamepad.wButtons &XINPUT_Buttons[i]) == XINPUT_Buttons[i];
	
		bGamepad_ButtonsDown[i] = !bPrev_ButtonsStates[i] && bButtonStates[i];
	}
}

void Gamepad::RefreshState()
{
	memcpy(bPrev_ButtonsStates, bButtonStates, sizeof(bPrev_ButtonsStates));
}

bool Gamepad::LStick_Dead()
{
	//gets x y axes of the stick
	short sX = m_State.Gamepad.sThumbLX;
	short sY = m_State.Gamepad.sThumbLY;

	//if y or x axis is out of dead zone
	if ((sX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) || (sX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
		return false;
	if ((sY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) || (sY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
		return false;
	//An axis or both are in the deadzone
	return true;
}

bool Gamepad::RStick_Dead()
{
	//gets x y axes of the stick
	short sX = m_State.Gamepad.sThumbRX;
	short sY = m_State.Gamepad.sThumbRY;

	//if x or y axis is out of dead zone
	if ((sX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) || (sX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
		return false;
	if ((sY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) || (sY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
		return false;
	//An axis or both are in the deadzone
	return true;
}

// May have some issues in the code below for aquiring the stick axis
// Return X axis of left stick
float Gamepad::LeftStick_X()
{
	// Obtain X axis of left stick
	short sX = m_State.Gamepad.sThumbLX;
	cout << "left stick x" << endl;
	// Return axis value, converted to a float
	return (static_cast<float>(sX) / 32768.0f);
}

// Return Y axis of left stick
float Gamepad::LeftStick_Y()
{
	// Obtain Y axis of left stick
	short sY = m_State.Gamepad.sThumbLY;
	cout << "left stick y" << endl;
	// Return axis value, converted to a float
	return (static_cast<float>(sY) / 32768.0f);
}

// Return X axis of right stick
float Gamepad::RightStick_X()
{
	// Obtain X axis of right stick
	short sX = m_State.Gamepad.sThumbRX;
	cout << "right stick x: " << static_cast<float>(sX) / 32768.0f << endl;
	// Return axis value, converted to a float
	return (static_cast<float>(sX) / 32768.0f);
}

// Return Y axis of right stick
float Gamepad::RightStick_Y()
{
	// Obtain the Y axis of the left stick
	short sY = m_State.Gamepad.sThumbRY;
	cout << "right stick y" << endl;
	// Return axis value, converted to a float
	return (static_cast<float>(sY) / 32768.0f);
}

//NOTE the division by 255.f is to scale down the range from 0-255 to 0-1 to get the pressure pushing down on the triggers
// Return value of left trigger
float Gamepad::LeftTrigger()
{
	// Obtain value of left trigger
	BYTE Trigger = m_State.Gamepad.bLeftTrigger;

	if (Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		cout << "Left trigger" << Trigger / 255.0f << endl;
		return Trigger / 255.0f;

	return 0.0f; // Trigger was not pressed
}

// Return value of right trigger
float Gamepad::RightTrigger()
{
	// Obtain value of right trigger
	BYTE Trigger = m_State.Gamepad.bRightTrigger;

	if (Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		cout << "Right trigger" << Trigger / 255.0f << endl;
		return Trigger / 255.0f;

	return 0.0f; // Trigger was not pressed
}

//the 65535f multiplication is to change the range of vibration to 0.0f to 1.0f
void Gamepad::Rumble(float a_fLeftMotor = 0.0f, float a_fRightMotor = 0.0f)
{
	XINPUT_VIBRATION vibrationState;

	ZeroMemory(&vibrationState, sizeof(XINPUT_VIBRATION));

	// Calculate vibration values
	int iLeftMotor = int(a_fLeftMotor * 65535.0f);
	int iRightMotor = int(a_fRightMotor * 65535.0f);

	// Set vibration values
	vibrationState.wLeftMotorSpeed = iLeftMotor;
	vibrationState.wRightMotorSpeed = iRightMotor;

	// Set the vibration state
	XInputSetState(m_iGamepadIndex, &vibrationState);
}

//call the function with the index of the button we want to check if pressed
bool Gamepad::GetButtonPressed(int a_iButton)
{
	if (m_State.Gamepad.wButtons & XINPUT_Buttons[a_iButton])
	{
		return true; //pressed
	}
	return false; //not pressed
}

bool Gamepad::GetButtonDown(int a_iButton)
{
	return bGamepad_ButtonsDown[a_iButton];
}


void Gamepad::glfwJoystick()
{
	int present = glfwJoystickPresent(GLFW_JOYSTICK_1);
	//std::cout << "Joystick 1 status: " << present << std::endl;

	if (1 == present)
	{
		int axesCount;
		const float *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
		
		//large amount of spaces to clarify inputs
		cout << " " << endl;
		cout << " " << endl;
		cout << " " << endl;
		cout << " " << endl;
		cout << " " << endl;
		cout << " " << endl;
		cout << " " << endl;

		cout << "Number of axes available" << axesCount << endl;

		//figure out which slot in the axes array corresponds to each stick axis and trigger then comment
		cout << "Left Stick X Axis" << axes[0] << endl;
		cout << "Left Stick Y Axis" << axes[1] << endl;
		cout << "Right Stick X Axis" << axes[2] << endl;
		cout << "Right Stick Y Axis" << axes[3] << endl;
		cout << "Left Trigger" << axes[4] << endl;
		cout << "Rght Trigger" << axes[5] << endl;
		
		int buttonCount;
		const unsigned char *buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);
		//0 is A button
		if (GLFW_PRESS == buttons[0])
		{
			cout << "Button Pressed" << endl;
		}
		else if (GLFW_RELEASE == buttons[0])
		{
			cout << "Button Released" << endl;
		}
		//1 is B button
		if (GLFW_PRESS == buttons[1])
		{
			cout << "Button Pressed" << endl;
		}
		else if (GLFW_RELEASE == buttons[1])
		{
			cout << "Button Released" << endl;
		}
		//2 is X button
		if (GLFW_PRESS == buttons[2])
		{
			cout << "Button Pressed" << endl;
		}
		else if (GLFW_RELEASE == buttons[2])
		{
			cout << "Button Released" << endl;
		}
		//3 is Y button
		if (GLFW_PRESS == buttons[3])
		{
			cout << "Button Pressed" << endl;
		}
		else if (GLFW_RELEASE == buttons[3])
		{
			cout << "Button Released" << endl;
		}
		
		const char *name = glfwGetJoystickName(GLFW_JOYSTICK_1);
		cout << "joystick is called: " << name << endl;
	}
}
