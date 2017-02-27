#include "Graphics.h"
#include "Resources.h"
#include "Game.h"
#include "PhysicsManager.h"
#include "Gamepad.h"
#include "InputManager.h"
#include "Main.h"

using namespace std;

// create gamepad instance (controller no. 1)
//Gamepad myGamepad = Gamepad(1);

//below for multiple gamepads
//Gamepad gamepad1 = Gamepad(1);
//Gamepad gamepad2 = Gamepad(2);

int main(int argc, char *argv[])
{

	if (Graphics::init() == -1)return -1;

	//init physics as well
	PhysicsManager::init();

	Light::init(2);
	Viewport::init(2);
	Resources::init();
	Time::init();
	Keyboard::init();
	Game::init();
	while (!Graphics::shouldClose()) {
		Time::update();
		Game::update();
		Graphics::update();
		
		//myGamepad.Update(); // Update the gamepad
		//myGamepad.GetState();
		PhysicsManager::update(1);

		Keyboard::update();
	}
	PhysicsManager::destroy();
	Graphics::destroy();
}
