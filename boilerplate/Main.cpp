#include "Graphics.h"
#include "Resources.h"
#include "Game.h"
#include "PhysicsManager.h"
#include "Gamepad.h"
#include "InputManager.h"
#include "Main.h"

using namespace std;

Gamepad gamepad = Gamepad(1);
Gamepad gamepad2 = Gamepad(2);

int main(int argc, char *argv[])
{

	if (Graphics::init() == -1)return -1;
	//PhysicsManager *mPhysx = new PhysicsManager();

	//mPhysx->initPhysX();
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
		InputManager::GetGamepadInput(gamepad);
		InputManager::GetGamepadInput(gamepad2);

	}
	Graphics::destroy();
}
