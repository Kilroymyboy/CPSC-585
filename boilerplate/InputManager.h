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
	static void GetGamepadInput(Gamepad myGamepad); //Getting gamepad input to be called in gameloop

	void mouse(GLFWwindow* window, int button, int action, int mods); 
	void motion(GLFWwindow* w, double x, double y);

};

namespace Keyboard {
	void init();
	void update();
	bool keyDown(int key);
	bool keyPressed(int key);
	bool keyReleased(int key);
	void Callback(GLFWwindow* window, int key, int scancode, int action, int mods);
}