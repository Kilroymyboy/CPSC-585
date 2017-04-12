#include "GameExt.h"
#include "Skybox.h"
#include "Menu.h"

using namespace std;

namespace Game {
	void clear() {
		alive = 0;
		entities.clear();
		aiPowerUps.clear();
	}

	void startMainMenu() {
		clear();

		entities.push_back(unique_ptr<Menu::All>(new Menu::All()));
		entities.push_back(unique_ptr<Plane>(new Plane));
	}

	void startSinglePlayer() {
		clear();

		Game::vs_ai = true;
		aventador0 = shared_ptr<Aventador>(new Aventador(0));
		aventador1 = shared_ptr<Aventador>(new Aventador(1));
		path = shared_ptr<Path>(new Path(150));
		entities.push_back(aventador0);
		entities.push_back(aventador1);
		entities.push_back(path);	//the path that gets drawn under the car

		entities.push_back(unique_ptr<Plane>(new Plane));
		entities.push_back(unique_ptr<Skybox>(new Skybox(1000)));
	}

	void startMultiPlayer() {
		clear();

		Game::vs_ai = false;
		aventador0 = shared_ptr<Aventador>(new Aventador(0));
		aventador1 = shared_ptr<Aventador>(new Aventador(1));
		path = shared_ptr<Path>(new Path(150));
		entities.push_back(aventador0);
		entities.push_back(aventador1);
		entities.push_back(path);	//the path that gets drawn under the car

		entities.push_back(unique_ptr<Plane>(new Plane));
		entities.push_back(unique_ptr<Skybox>(new Skybox(1000)));
	}
}