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
	shared_ptr<Aventador> aventador;

	// we can customize this function as much as we want for now for debugging
	void init() {
		aventador = shared_ptr<Aventador>(new Aventador);
		entities.push_back(aventador);
	//	entities.push_back(shared_ptr<Path>(new Path(100)));
		//	entities.push_back(unique_ptr<Cube>(new Cube));
	//	entities.push_back(unique_ptr<CenteredCube>(new CenteredCube(vec3(0, 3, 0))));
		entities.push_back(unique_ptr<Plane>(new Plane));
	}

	void update() {
		glfwPollEvents();

		for (auto it = entities.begin(); it != entities.end(); it++) {
			if (it->get()->alive) {
				it->get()->update0(mat4(1));
			}
			else {
				it = entities.erase(it);
			}
		}

		for (auto it = entities.begin(); it != entities.end(); it++) {
			it->get()->render(mat4(1));
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
