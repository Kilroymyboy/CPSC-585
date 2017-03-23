#include "Game.h"
#include "Resources.h"
#include "InputManager.h"
#include "Aventador.h"
#include "Path.h"
#include "Skybox.h"

using namespace std;
using namespace glm;
using namespace physx;

namespace Game {
	list<shared_ptr<Entity>> entities;
	shared_ptr<Aventador> aventador0;
	shared_ptr<Aventador> aventador1;
	shared_ptr<Path> path;

	shared_ptr<HUDobj> hud;

	double spawnCoolDown = 2;
	double powerUpSpawnTime = Time::time += spawnCoolDown;
	float impulse = 300;
	double switchRange = 15.0;
	bool inSwtichRange = false;

	// we can customize this function as much as we want for now for debugging
	void init() {
		aventador0 = shared_ptr<Aventador>(new Aventador(0));
		aventador1 = shared_ptr<Aventador>(new Aventador(1));
		path = shared_ptr<Path>(new Path(100));
		entities.push_back(aventador0);
		entities.push_back(aventador1);
		entities.push_back(path);	//the path that gets drawn under the car

	//	entities.push_back(shared_ptr<Path>(new Path(100, aventador0)));	//the path that gets drawn under the car

		//entities.push_back(unique_ptr<Cube>(new Cube));
		//entities.push_back(unique_ptr<CenteredCube>(new CenteredCube(vec3(0, 3, 0))));
	//	entities.push_back(unique_ptr<Plane>(new Plane));
	//	entities.push_back(unique_ptr<Skybox>(new Skybox(1000)));

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
		if (PRINT_ENTITIES) {
			cout << entities.size() << endl;
		}

		//adding more power ups into the scene
		if (Time::time > powerUpSpawnTime) {
			powerUpSpawnTime += spawnCoolDown;
			entities.push_back(shared_ptr<Entity>(new PowerUpManager()));
		}
		//check the distance between the aventators
		if (dist < switchRange && !inSwtichRange) {
			PxRigidBodyExt::addLocalForceAtLocalPos(*getBack()->actor,
				PxVec3(0, 0, impulse), PxVec3(0, 0, 0), PxForceMode::eIMPULSE);
			switchRole();
			inSwtichRange = true;
		}
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
		PxTransform pos0 = aventador0->actor->getGlobalPose();
		PxTransform pos1 = aventador1->actor->getGlobalPose();
		return sqrt(pow((pos0.p.x - pos1.p.x), 2) + pow((pos0.p.z - pos1.p.z), 2));
	}

	Aventador* getFront() {
		return (aventador0->isFront()) ? aventador0.get() : aventador1.get();
	}

	Aventador* getBack() {
		return (aventador0->isFront()) ? aventador1.get() : aventador0.get();
	}

	bool cnPointPolyTest(float x, float y, std::vector<glm::vec3> V, int n) {
		int    cn = 0;    // the  crossing number counter
						  // loop through all edges of the polygon
		for (int i = 0; i<n-1; i++) {    // edge from V[i]  to V[i+1]
			if (((V[i].z <= y) && (V[i + 1].z > y))     // an upward crossing
				|| ((V[i].z > y) && (V[i + 1].z <= y))) { // a downward crossing
															  // compute  the actual edge-ray intersect x-coordinate
				float vt = (float)(y - V[i].z) / (V[i + 1].z - V[i].z);
				if (x <  V[i].x + vt * (V[i + 1].x - V[i].x)) // x < intersect
					++cn;   // a valid crossing of y=y right of x
			}
		}
		return (cn & 1);    // 0 if even (out), and 1 if  odd (in)
	}

	inline int isLeft(vec3 P0, vec3 P1, float x, float y)
	{
		return ((P1.x - P0.x) * (y - P0.z) - (x - P0.x) * (P1.z - P0.z));
	}

	int wnPointPolyTest(float x, float y, vector<glm::vec3> V, int n)
	{
		int    wn = 0;    // the  winding number counter

						  // loop through all edges of the polygon
		for (int i = 0; i<n-1; i++) {   // edge from V[i] to  V[i+1]
			if (V[i].z <= y) {          // start y <= P.y
				if (V[i + 1].z  > y)      // an upward crossing
				{
					int l = isLeft(V[i], V[i + 1], x, y);
					if (l > 0)  // P left of  edge
						++wn;            // have  a valid up intersect
					else if (l == 0) // boundary
						return 0;
				}
			}
			else {                        // start y > P.y (no test needed)
				if (V[i + 1].z <= y)     // a downward crossing
				{
					int l = isLeft(V[i], V[i + 1], x, y);
					if (l < 0)  // P right of  edge
						--wn;            // have  a valid down intersect
					else if (l == 0)
						return 0;
				}
			}
		}
		return wn;
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