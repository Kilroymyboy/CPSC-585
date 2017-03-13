#include "Graphics.h"
#include "Resources.h"
#include "Game.h"
#include "PhysicsManager.h"
#include "Gamepad.h"
#include "InputManager.h"
#include "Main.h"

using namespace std;

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
		
		//need these 4 lines for controller update now that its global
		controller1.Update();
		controller2.Update();
		controller1.GetState();
		controller2.GetState();

		PhysicsManager::update(1);

		Keyboard::update();
	}
	PhysicsManager::destroy();
	Graphics::destroy();
}
