#include "Game.h"
#include "Resources.h"
#include "InputManager.h"
#include "Aventador.h"
#include "Path.h"

using namespace std;
using namespace glm;
using namespace physx;

namespace Game {
	list<shared_ptr<Entity> > entities;
	shared_ptr<Aventador> aventador0;
	shared_ptr<Aventador> aventador1;

	shared_ptr<HUDobj> hud;

	double spawnCoolDown = 1.0;
	double powerUpSpawnTime = Time::time += spawnCoolDown;


	// we can customize this function as much as we want for now for debugging
	void init() {
		aventador0 = shared_ptr<Aventador>(new Aventador(0));

		aventador1 = shared_ptr<Aventador>(new Aventador(1));
		entities.push_back(aventador0);
		entities.push_back(shared_ptr<Path>(new Path(100, 1, 0)));	//the path that gets drawn under the car
		entities.push_back(aventador1);

		//entities.push_back(unique_ptr<Plane>(new Plane));
	}

	int update() {
		glfwPollEvents();
		for (auto it = entities.begin(); it != entities.end(); it++) {
			if (it->get()->alive) {
				it->get()->update(mat4(1));
			}
			else {
				it = entities.erase(it);
			}
		}
		//adding more power ups into the scene
		if (Time::time > powerUpSpawnTime) {
			powerUpSpawnTime += spawnCoolDown;
			entities.push_back(shared_ptr<Entity>(new PowerUp()));
		}

		//This is where a restart function would go
		//currently doing something wrong as restarting must not actually delete as the program slows down after each restart
		if ((controller1.GetButtonPressed(13)) || (Keyboard::keyPressed(GLFW_KEY_Q))) {
			entities.clear();
			init();
		}
		else if (controller1.GetButtonPressed(12)) {
			return 2;
		}
	}

	void switchRole() {
		aventador0->changeRole();
		aventador1->changeRole();
	}

	//this is it's own game loop so this would generate the start screen objects or place an image
	int startScreen() {
		//initialization of whatever we want in here for now just a print statement
		//currently can only go to the main game loop because of how i set up loops in the main file
		controller1.Update();
		controller2.Update();
		controller1.GetState();
		controller2.GetState();
		cout << "In Start Screen" << endl;
		if ((controller1.GetButtonPressed(12)) || (Keyboard::keyPressed(GLFW_KEY_ENTER))) {
			cout << "ENTERED GAME LOOP" << endl;
			entities.clear();
			init();
			return 1;
		}
	}

	//same as start screen but just after the race is over
	void endScreen() {
		entities.clear();
	}
}

namespace Time {
	double time, prevTime, deltaTime, fpsTime;
	int fps, tfps;

	void init() {
		time = glfwGetTime();
		prevTime = 0;
		deltaTime = 0;
		fps = tfps = 0;
		fpsTime = 0;
	}

	void update() {
		time = glfwGetTime();
		deltaTime = time - prevTime;
		prevTime = time;
		if (time > fpsTime + 1) {
			fps = tfps;
			if (PRINT_FPS) {
				cout << "fps: " << fps << endl;
			}
			tfps = 0;
			fpsTime = time;
		}
		else {
			tfps++;
		}
	}
}