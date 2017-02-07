#include "Graphics.h"
#include "Gamepad.h"
#include "Main.h"

using namespace std;

// create gamepad instance (controller no. 1)
Gamepad gamepad;
// A function to obtain input, called each frame
void GetGamepadInput()
{
	//NOTE gamepad update() must be called BEFORE any pulling inputs
	gamepad.Update(); // Update the gamepad

	if (gamepad.GetButtonPressed(Xbuttons.A)) {
		cout << "A pressed" << endl;
	}

	else if (gamepad.GetButtonDown(Xbuttons.X)) {
		cout << "X pressed " << endl;
	}

	else if (gamepad.GetButtonDown(Xbuttons.Y)) {
		cout << "Y pressed" << endl;
	}
	gamepad.RightTrigger();
	gamepad.LeftTrigger();

	// Update the gamepad for next frame
	gamepad.RefreshState();
}

void glfwJoystick()
{
	int present = glfwJoystickPresent(GLFW_JOYSTICK_1);
	std::cout << "Joystick 1 status: " << present << std::endl;

	if (1 == present)
	{
		int axesCount;
		const float *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
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

		if (GLFW_PRESS == buttons[0])
		{
			cout << "Button Pressed" << endl;
		}
		else if (GLFW_RELEASE == buttons[1])
		{
			cout << "Button Released" << endl;
		}

		const char *name = glfwGetJoystickName(GLFW_JOYSTICK_1);
		cout << "joystick is called: " << name << endl;
	}
}

int main(int argc, char *argv[])
{

	if (Graphics::init() == -1)return -1;
	Light::init();
	Viewport::init();

	while (!Graphics::shouldClose()) {
		Graphics::update();
		glfwJoystick();
	}
	Graphics::destroy();
}



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