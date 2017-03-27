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

Gamepad controller1 = Gamepad(1);
Gamepad controller2 = Gamepad(2);

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