#include "Game.h"
#include "Resources.h"
#include "InputManager.h"
#include "Aventador.h"
#include "Path.h"

using namespace std;
using namespace glm;
using namespace physx;

namespace Game {
	list<shared_ptr<Entity>> entities;
	shared_ptr<Aventador> aventador0;
	shared_ptr<Aventador> aventador1;
	shared_ptr<Path> path;

	shared_ptr<HUDobj> hud;

	double spawnCoolDown = 1;
	double powerUpSpawnTime = Time::time += spawnCoolDown;
	float impulse = 300;
	double switchRange = 15.0;
	bool inSwtichRange = false;

	// we can customize this function as much as we want for now for debugging
	void init() {
		aventador0 = shared_ptr<Aventador>(new Aventador(0));
		aventador1 = shared_ptr<Aventador>(new Aventador(1));
		path = shared_ptr<Path>(new Path(100, 1, 0));
		entities.push_back(aventador0);
		entities.push_back(path);	//the path that gets drawn under the car
		entities.push_back(aventador1);

		//entities.push_back(unique_ptr<Plane>(new Plane));
	}

	void update() {
		glfwPollEvents();
		double dist = getDist();

		for (auto it = entities.begin(); it != entities.end(); it++) {
			if (it->get()->alive) {
				it->get()->update(mat4(1));
			}
			else {
				it = entities.erase(it);
			}
		}
		//if the back aventator is not on the path
		if (!isOnPath()) {
			//reduce fuel/hp on aventador
			//std::cout << "back aventador is not on the path\n";

		}
		else {
			// fuel/hp is full
			std::cout << "back aventador is on the path\n";
		}
		//adding more power ups into the scene
		if (Time::time > powerUpSpawnTime) {
			powerUpSpawnTime += spawnCoolDown;
			entities.push_back(shared_ptr<Entity>(new PowerUpManager()));
		}
		//check the distance between the aventators
		if (dist < switchRange && !inSwtichRange) {
			PxRigidBodyExt::addLocalForceAtLocalPos(*getBack()->getActor(),
				PxVec3(0, 0, impulse), PxVec3(0, 0, 0), PxForceMode::eIMPULSE);
			switchRole();
			inSwtichRange = true;
		}
		//to change the role only once
		else if (dist > switchRange && inSwtichRange) {
			inSwtichRange = false;
		}

	}

	void switchRole() {
		aventador0->changeRole();
		aventador1->changeRole();
		std::cout << "role switch\n";
	}

	double getDist() {
		PxTransform pos0 = aventador0->getActor()->getGlobalPose();
		PxTransform pos1 = aventador1->getActor()->getGlobalPose();
		return sqrt(pow((pos0.p.x - pos1.p.x), 2) + pow((pos0.p.z - pos1.p.z), 2));
	}

	Aventador* getFront() {
		return (aventador0->isFront()) ? aventador0.get() : aventador1.get();
	}

	Aventador* getBack() {
		return (aventador0->isFront()) ? aventador1.get() : aventador0.get();
	}

	bool isOnPath() {
		std::vector<glm::vec3> backWheels = getBack()->wheelPos;
		std::vector<glm::vec3> pathPos = path->getPathPoints();
		for (std::vector<glm::vec3>::iterator it = backWheels.begin(); it != backWheels.end(); ++it) {
			if (std::find(pathPos.begin(), pathPos.end(), *it) != pathPos.end()) {
				return true;
			}
		}
		return false;
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