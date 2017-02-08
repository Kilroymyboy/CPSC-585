#include "Graphics.h"
#include "Gamepad.h"
#include "Main.h"

using namespace std;

// create gamepad instance (controller no. 1)
Gamepad myGamepad = Gamepad(1);
// A function to obtain input, called each frame
void GetGamepadInput()
{
	//NOTE gamepad update() must be called BEFORE any pulling inputs
	myGamepad.Update(); // Update the gamepad
	myGamepad.GetIndex();
	if (myGamepad.GetButtonPressed(Xbuttons.A) == true) {
		cout << "A pressed" << endl;
	}

	else if (myGamepad.GetButtonDown(Xbuttons.X) == true) {
		cout << "X pressed " << endl;
	}

	else if (myGamepad.GetButtonDown(Xbuttons.Y) == true) {
		cout << "Y pressed" << endl;
	}
	else if (myGamepad.LeftTrigger() > 0)
	{
		cout << "TRIGGERED" << endl;
	}

	// Update the gamepad for next frame
	myGamepad.RefreshState();
}

int main(int argc, char *argv[])
{

	if (Graphics::init() == -1)return -1;
	Light::init();
	Viewport::init();
	
	while (!Graphics::shouldClose()) {
		Graphics::update();
		myGamepad.glfwJoystick();
		//GetGamepadInput();
	}
	Graphics::destroy();
}
