#include "Graphics.h"
#include "Resources.h"
#include "Game.h"
#include "PhysicsManager.h"
#include "Gamepad.h"
#include "InputManager.h"
#include "Main.h"
#include "HUD.h"
//#include "Sound.h"


using namespace std;

HUD *hud;
bool startScreen = true;

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
	//Sound::init();
	//Sound::playSound(1);
	
//	Sound::list_audio_devices(Sound::device);
	while (!Graphics::shouldClose()) {
		
		while (startScreen == true) {
			
			Graphics::update();

			controller1.Update();
			controller2.Update();
			controller1.GetState();
			controller2.GetState();

			if ((controller1.GetButtonPressed(0))) {
				startScreen = false;
			}

		}
		Time::update();
		Game::update();
		Graphics::update();
		hud->update();
		
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
