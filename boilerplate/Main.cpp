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

int main(int argc, char *argv[])
{

	if (Graphics::init() == -1)return -1;
	Light::init();
	Viewport::init();
	
	while (!Graphics::shouldClose()) {
		Graphics::update();
		gamepad.glfwJoystick();
	}
	Graphics::destroy();
}
