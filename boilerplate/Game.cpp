#include "Game.h"
#include "Resources.h"
#include "InputManager.h"
#include "Aventador.h"

using namespace std;
using namespace glm;
using namespace physx;

namespace Game {
	list<unique_ptr<Entity>> entities;

	// we can customize this function as much as we want for now for debugging
	void init() {
		entities.push_back(unique_ptr<Aventador>(new Aventador));
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
			it->get()->update(mat4(1));
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

void Plane::update0(glm::mat4 parentTransform) {
	for (int i = -5; i < 6; i++) {
		for (int j = -5; j < 6; j++) {
			mat4 t = translate(transform, vec3(i * 16, 0, j * 16));
			Light::renderShadowMap(&Resources::plane, parentTransform*t);
		}
	}
}

void Plane::update(glm::mat4 parentTransform) {
	for (int i = -3; i < 2; i++) {
		for (int j = -3; j < 2; j++) {
			mat4 t = translate(transform, vec3(i * 16, 0, j * 16));
			Graphics::RenderScene(&Resources::plane, &Resources::standardShader, &Resources::defaultMaterial, parentTransform*t);
		}
	}
}

void Cube::update0(glm::mat4 parentTransform) {
	Light::renderShadowMap(&Resources::cube, parentTransform*transform);
}

void Cube::update(glm::mat4 parentTransform) {
	Graphics::RenderScene(&Resources::cube, &Resources::standardShader, &Resources::defaultMaterial, parentTransform*transform);
}

CenteredCube::CenteredCube(vec3 position) {
	PxTransform t(PxVec3(position.x, position.y, position.z), PxQuat::createIdentity());

	PxVec3 dimensions(0.5f, 0.5f, 0.5f);
	actor = PhysicsManager::createDynamic(t, dimensions);
}

void CenteredCube::update0(glm::mat4 parentTransform) {

	glm::mat4 m = glm::translate(glm::mat4(1), glm::vec3(actor->getGlobalPose().p.x, actor->getGlobalPose().p.y, actor->getGlobalPose().p.z));
	PxReal a; PxVec3 b;  actor->getGlobalPose().q.toRadiansAndUnitAxis(a, b); m = glm::rotate(m, (float)a, glm::vec3(b.x, b.y, b.z));
	transform = m;

	Light::renderShadowMap(&Resources::centeredCube, transform);
}

void CenteredCube::update(glm::mat4 parentTransform) {
	Graphics::RenderScene(&Resources::centeredCube, &Resources::standardShader, &Resources::defaultMaterial, transform);
}