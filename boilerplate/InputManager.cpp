#include "InputManager.h"
#include "Gamepad.h"

#include <vector>

using namespace std;

InputManager::InputManager()
{
}


InputManager::~InputManager()
{
}

//has wasd and qe for keys mapped if we want player 2 on keyboard since the numpad mapping is actually just 1=a and so on up to 9 will have to use some other mapping
void InputManager::keyBoard(unsigned char key, int x, int y)
{
	float factor = 0.05f;

	switch (key)
	{
	case (27):
		exit(EXIT_FAILURE);

	case (GLFW_KEY_A):
		cout << " A pressed " << endl;
		break;

	case (GLFW_KEY_D):
		cout << " D pressed " << endl;
		break;

	case (GLFW_KEY_S):
		cout << " S pressed " << endl;
		break;

	case (GLFW_KEY_W):
		cout << " W pressed " << endl;
		break;

	case (GLFW_KEY_Q):
		cout << " Q pressed " << endl;
		break;

	case (GLFW_KEY_E):
		cout << " E pressed " << endl;
		break;

	case (GLFW_KEY_J):
		cout << " J " << endl;
		break;

	case (GLFW_KEY_K):
		cout << " K pressed " << endl;
		break;

	case (GLFW_KEY_L):
		cout << " L pressed " << endl;
		break;

	case (GLFW_KEY_I):
		cout << " I pressed " << endl;
		break;

	case (GLFW_KEY_U):
		cout << " U pressed " << endl;
		break;

	case (GLFW_KEY_O):
		cout << " O pressed " << endl;
		break;
	}
}

// A function to obtain input, called each frame
void InputManager::GetGamepadInput(Gamepad myGamepad)
{
	//NOTE gamepad update() must be called BEFORE any pulling inputs
	myGamepad.Update(); // Update the gamepad
	myGamepad.GetState();

	if (myGamepad.GetButtonPressed(Xbuttons.A)) {
		cout << "A pressed" << endl;
	}
	else if (myGamepad.GetButtonPressed(Xbuttons.X)) {
		cout << "X pressed " << endl;
	}
	else if (myGamepad.GetButtonPressed(Xbuttons.Y)) {
		cout << "Y pressed" << endl;
	}
	else if (myGamepad.GetButtonPressed(Xbuttons.B)) {
		cout << "B pressed" << endl;
	}
	else if (myGamepad.LeftTrigger() > 0)
	{
		cout << "Left Trigger " << endl;
	}
	else if (myGamepad.RightTrigger() > 0)
	{
		cout << "Right Trigger " << endl;
	}
	else if (myGamepad.GetButtonPressed(Xbuttons.Start))
	{
		cout << "GAME START!!!!" << endl;
	}

	//four test calls to get stick x y every time implement it later for what we need to control the car but they all work
	//myGamepad.RightStick_X();
	//myGamepad.RightStick_Y();
	//myGamepad.LeftStick_X();
	//myGamepad.LeftStick_Y();

	// Update the gamepad for next frame
	myGamepad.RefreshState();

}

/*
//Old mouse code currently unaware of what to do with it so storing it here for now
void InputManager::mouse(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		mouse_old_x = x;
		mouse_old_y = y;
	}
}

void InputManager::motion(GLFWwindow* w, double x, double y)
{

	double dx, dy;
	dx = (x - mouse_old_x);
	dy = (y - mouse_old_y);

	//movement for both rotations x and y
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1))
	{
		rotate_x += dy * 0.5f;
		rotate_y += dx * 0.5f;
	}
	//same as the other mouse button full control on both buttons
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2))
	{
		rotate_x += dy * 0.5f;
		rotate_y += dx * 0.5f;
	}

	mouse_old_x = x;
	mouse_old_y = y;
}*/

namespace Keyboard {
	vector<bool> v, pv;
	void init() {
		v.assign(349, 0);
		pv.assign(349, 0);
	}

	void update() {
		for (int i = 0; i < 349; i++) pv[i] = v[i];
	}

	bool keyDown(int key) {
		return pv[key];
	}

	bool keyPressed(int key) {
		return v[key] && !pv[key];
	}

	bool keyReleased(int key) {
		return !v[key] && pv[key];
	}

	void Callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (key < 0 || key >= 349)return;
		if (action == GLFW_PRESS) {
			v[key] = 1;
		}
		else if (action == GLFW_RELEASE) {
			v[key] = 0;
		}
	}
}