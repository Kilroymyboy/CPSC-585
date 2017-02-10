#include "Graphics.h"
#include "Resources.h"
#include "Game.h"
#include "PhysicsManager.h"
#include "Gamepad.h"
#include "Main.h"

using namespace std;

// create gamepad instance (controller no. 1)
Gamepad myGamepad = Gamepad(1);
 
// A function to obtain input, called each frame
void GetGamepadInput()
{
	//NOTE gamepad update() must be called BEFORE any pulling inputs

	if (myGamepad.GetButtonPressed(Xbuttons.A)) {
		cout << "A pressed" << endl;
	}
	else if (myGamepad.GetButtonPressed(Xbuttons.X)) {
		cout << "X pressed " << endl;
	}
	else if (myGamepad.GetButtonPressed(Xbuttons.Y) ) {
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

int main(int argc, char *argv[])
{

	if (Graphics::init() == -1)return -1;
	PhysicsManager *mPhysx = new PhysicsManager();

	Light::init();
	Viewport::init();
	Resources::init();
	Time::init();
	Game::init();
	
	while (!Graphics::shouldClose()) {
		Graphics::clearFrameBuffer();
		Time::update();
		Game::update();
		Graphics::update();
		//myGamepad.glfwJoystick();
		myGamepad.Update(); // Update the gamepad
		myGamepad.GetState();
		GetGamepadInput();
	}
	Graphics::destroy();
}
