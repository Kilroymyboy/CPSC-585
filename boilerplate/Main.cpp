#include "Graphics.h"
#include "Resources.h"
#include "Game.h"
#include "PhysicsManager.h"
#include "Gamepad.h"
#include "InputManager.h"
#include "Main.h"
#include "HUD.h"
#include "Sound.h"


using namespace std;

// create gamepad instance (controller no. 1)
Gamepad myGamepad = Gamepad(1);
HUD *hud;

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

Gamepad gamepad = Gamepad(1);
Gamepad gamepad2 = Gamepad(2);

int main(int argc, char *argv[])
{

	if (Graphics::init() == -1)return -1;

	//init physics as well
	PhysicsManager::init();
	hud = new HUD();
	Light::init(2);
	Viewport::init(2);
	Resources::init();
	Time::init();
	Keyboard::init();
	Game::init();
	Sound::init();
	Sound::playSound(1);
	
//	Sound::list_audio_devices(Sound::device);
	while (!Graphics::shouldClose()) {
		Time::update();
		Game::update();
		Graphics::update();
		hud->update();
		
		//myGamepad.glfwJoystick();
		myGamepad.Update(); // Update the gamepad
		myGamepad.GetState();
		GetGamepadInput();
		PhysicsManager::update(1);

		InputManager::GetGamepadInput(gamepad);
		InputManager::GetGamepadInput(gamepad2);

		Keyboard::update();
	}
	PhysicsManager::destroy();
	Graphics::destroy();
}
