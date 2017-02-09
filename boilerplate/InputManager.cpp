#include "InputManager.h"

using namespace std;

InputManager::InputManager()
{
}


InputManager::~InputManager()
{
}

//has wasd and qe for keys mapped currently don't know where to put
void InputManager::keyBoard(unsigned char key, int x, int y)
{
	float factor = 0.05f;

	cout << key << endl;
	
	switch (key)
	{
	case (27) :
		exit(EXIT_FAILURE);

	case (GLFW_KEY_A) :
		cout << " A pressed " << endl;
		break;

	case (GLFW_KEY_D) :
		cout << " D pressed " << endl;
		break;

	case (GLFW_KEY_S) :
		cout << " S pressed " << endl;
		break;

	case (GLFW_KEY_W) :
		cout << " W pressed " << endl;
		break;

	case (GLFW_KEY_Q) :
		cout << " Q pressed " << endl;
		break;

	case (GLFW_KEY_E) :
		cout << " E pressed " << endl;
		break;

		//These don't work our keyboards don't even return numpad keys
	case (GLFW_KEY_KP_4) :
		cout << " numpad 4 " << endl;
		break;

	case (GLFW_KEY_KP_5) :
		cout << " numpad 5 " << endl;
		break;

	case (GLFW_KEY_KP_6) :
		cout << " numpad 6 " << endl;
		break;

	case (GLFW_KEY_KP_7) :
		cout << " numpad 7 " << endl;
		break;

	case (GLFW_KEY_KP_8) :
		cout << " numpad 8 " << endl;
		break;

	case (GLFW_KEY_KP_9) :
		cout << " numpad 9 " << endl;
		break;
	}
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