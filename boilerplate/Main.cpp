#include "Graphics.h"
#include "Resources.h"
#include "Game.h"
#include "PhysicsManager.h"
#include "Gamepad.h"
#include "InputManager.h"
#include "Main.h"

using namespace std;

//initialize how many players we want can probably put this somewhere else
Gamepad gamepad = Gamepad(1);
Gamepad gamepad2 = Gamepad(2);

int main(int argc, char *argv[])
{

	if (Graphics::init() == -1)return -1;

	//init physics as well
	PhysicsManager *mPhysx = new PhysicsManager();

	Light::init();
	Viewport::init();
	Resources::init();
	Time::init();
	Game::init();
	PxU64 frameNum = 0;
	while (!Graphics::shouldClose()) {
		Graphics::clearFrameBuffer();
		Time::update();
		Game::update();
		Graphics::update();
		
		//myGamepad.glfwJoystick();
		mPhysx->update(frameNum++);

		InputManager::GetGamepadInput(gamepad);
		InputManager::GetGamepadInput(gamepad2);
	}
	Graphics::destroy();
}
