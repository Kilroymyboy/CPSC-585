#pragma once

#include <GLFW/glfw3.h>
#include <iostream>

class InputManager
{
public:
	InputManager();
	~InputManager();

	static void keyBoard(unsigned char key, int x, int y);
	void mouse(GLFWwindow* window, int button, int action, int mods);
	void motion(GLFWwindow* w, double x, double y);
};

