#pragma once

#include <GLFW/glfw3.h>
#include <iostream>
#include "Gamepad.h"

class InputManager
{
public:
	InputManager();
	~InputManager();

	static void keyBoard(unsigned char key, int x, int y); //Keyboard input from glfw


};

namespace Keyboard {
	void init();
	void update();
	bool keyDown(int key);
	bool keyPressed(int key);
	bool keyReleased(int key);
	void Callback(GLFWwindow* window, int key, int scancode, int action, int mods);
}


extern Gamepad controller1;
extern Gamepad controller2;