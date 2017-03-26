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
bool pause = false;

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
	//Sound::playSound(1);
	
//	Sound::list_audio_devices(Sound::device);
	while (!Graphics::shouldClose()) {
		
		//pauses the game while holding down start not ideal
		if ((controller1.GetButtonPressed(12)) || (pause == false)) {
			pause = true;
			cout << "game shoudl be paused" << pause << endl;
		}
		else if ((controller1.GetButtonPressed(0)) || (pause == true)) {
			pause = false;
			cout << "Game isn't paused " << pause << endl;
		}
		if (pause == false) {
			cout << "game loop" << endl;
			Time::update();
			Game::update();
			Graphics::update();
		
			hud->update();
			PhysicsManager::update(1);
		}
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
