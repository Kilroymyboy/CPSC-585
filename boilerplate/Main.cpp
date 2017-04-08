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

HUD *hud;
static int mode = 0;

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
	//Game::startScreen();
	Game::init();

	//Sound::init();
	
//	Sound::list_audio_devices(Sound::device);

	while (!Graphics::shouldClose()) {
		

		/*if (mode == 0) {
			cout << "In start screen" << endl;
			mode = Game::startScreen();
		}
		else if (mode == 1) {
			Time::update();
			Game::update();
			Graphics::update();

			hud->update();
			PhysicsManager::update(1);
		}*/
		/*
		Sound::playSound(1);
		Sound::playSound(2);
		Sound::updateSources();
		Sound::updateListener();
		*/


		Time::update();
		Game::update();
		Graphics::update();
		
		hud->update();
		PhysicsManager::update(1);
		hud->update();
		PhysicsManager::update(1);

		//need these 4 lines for controller update now that its global
		controller1.Update();
		controller2.Update();
		controller1.GetState();
		controller2.GetState();
	
		Keyboard::update();

	}
	PhysicsManager::destroy();
	Graphics::destroy();
}
