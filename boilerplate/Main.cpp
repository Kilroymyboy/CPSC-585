#include "Graphics.h"
#include "Gamepad.h"
#include "Main.h"

using namespace std;

// create gamepad instance (controller no. 1)
Gamepad MyGamepad = Gamepad(1);

// A function to obtain input, called each frame
void GetGamepadInput()
{
	//NOTE gamepad update() must be called BEFORE any pulling inputs
	MyGamepad.Update(); // Update the gamepad

	if (MyGamepad.GetButtonPressed(Xbuttons.A)) {
		cout << "A pressed" << endl;
	}

	else if (MyGamepad.GetButtonDown(Xbuttons.X)) {
		cout << "X pressed " << endl;
	}

	else if (MyGamepad.GetButtonDown(Xbuttons.Y)) {
		cout << "Y pressed" << endl;
	}

	// Update the gamepad for next frame
	MyGamepad.RefreshState();
}

int main(int argc, char *argv[])
{
	Gamepad gamepad;
	if (Graphics::init() == -1)return -1;
	Light::init();
	Viewport::init();

	GetGamepadInput();
	while (!Graphics::shouldClose()) {
		Graphics::update();
		GetGamepadInput();
	}
	Graphics::destroy();
}

/*
//has wasd and qe for keys mapped currently don't know where to put
void keyboard(unsigned char key, int x, int y)
{
	float factor = 0.05f;

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

	default:
		break;
	}
}
*/

/* Old mouse code currently unaware of what to do with it so storing it here for now
void mouse(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		mouse_old_x = x;
		mouse_old_y = y;
	}
}

void motion(GLFWwindow* w, double x, double y)
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

}
*/