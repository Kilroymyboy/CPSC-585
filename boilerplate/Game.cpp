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
	double spawnCoolDown = 5.0;
	double powerUpSpawnTime = Time::time += spawnCoolDown;

	// we can customize this function as much as we want for now for debugging
	void init() {
		aventador0 = shared_ptr<Aventador>(new Aventador(0));
		aventador1 = shared_ptr<Aventador>(new Aventador(1));
		entities.push_back(aventador0);
		entities.push_back(aventador1);
		entities.push_back(shared_ptr<Path>(new Path(100)));	//the path that gets drawn under the road
		entities.push_back(shared_ptr<PowerUp>(new PowerUp()));

		//entities.push_back(unique_ptr<Cube>(new Cube));
		//entities.push_back(unique_ptr<CenteredCube>(new CenteredCube(vec3(0, 3, 0))));
	//	entities.push_back(unique_ptr<Plane>(new Plane));

	}

	void update() {
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