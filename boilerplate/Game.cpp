#include "Game.h"
#include "Resources.h"
#include "InputManager.h"

using namespace std;
using namespace glm;
using namespace physx;

Aventador::Aventador() {
	wheel1 = std::unique_ptr<AventadorWheel>(new AventadorWheel);
	wheel1.get()->transform = translate(mat4(1), vec3(.851f, .331f, 1.282f));
	wheel2 = std::unique_ptr<AventadorWheel>(new AventadorWheel);
	wheel2.get()->transform = scale(translate(mat4(1), vec3(.858f, .356f, -1.427f)), vec3(1.07f, 1.07f, 1.07f));

	wheel1.get()->rotateSpeed = .1f;
	wheel2.get()->rotateSpeed = .1f;

	wheel0 = std::unique_ptr<AventadorWheel>(new AventadorWheel);
	wheel0.get()->transform = rotate(translate(mat4(1), vec3(-.851f, .331f, 1.282f)), (float)PI, vec3(0, 1, 0));
	wheel3 = std::unique_ptr<AventadorWheel>(new AventadorWheel);
	wheel3.get()->transform = scale(rotate(translate(mat4(1), vec3(-.858f, .356f, -1.427f)), (float)PI, vec3(0, 1, 0)), vec3(1.07f, 1.07f, 1.07f));

	wheel0.get()->rotateSpeed = -.1f;
	wheel3.get()->rotateSpeed = -.1f;

	modelDisplacement = vec3(0, -0.55, 0);

	PxTransform t(PxVec3(0, 2, 0), PxQuat::createIdentity());
	PxVec3 dimensions(1, 0.5, 2.5);
	PxBoxGeometry geometry(dimensions);
	actor = PxCreateDynamic(*PhysicsManager::mPhysics, t, geometry, *PhysicsManager::mPhysics->createMaterial(0.1f, 0.1f, 0.5f), PxReal(1.0f));
	actor->setAngularDamping(0.1);
	PhysicsManager::mScene->addActor(*actor);
}

void Aventador::update0(glm::mat4 parentTransform) {
	glm::mat4 m = glm::translate(glm::mat4(1), glm::vec3(actor->getGlobalPose().p.x, actor->getGlobalPose().p.y, actor->getGlobalPose().p.z));
	PxReal a; PxVec3 b;  actor->getGlobalPose().q.toRadiansAndUnitAxis(a, b); m = glm::rotate(m, (float)a, glm::vec3(b.x, b.y, b.z));
	transform = m;
	mat4 t = translate(transform, modelDisplacement);



	Light::renderShadowMap(&Resources::aventadorBody, parentTransform* t);
	Light::renderShadowMap(&Resources::aventadorBodyGlow, parentTransform*t);
	Light::renderShadowMap(&Resources::aventadorUnder, parentTransform*t);

	wheel1.get()->update0(parentTransform*t);
	wheel2.get()->update0(parentTransform*t);
	wheel0.get()->update0(parentTransform*t);
	wheel3.get()->update0(parentTransform*t);
}

void Aventador::update(glm::mat4 parentTransform) {
	mat4 t = translate(transform, modelDisplacement);

	Graphics::RenderScene(&Resources::aventadorBody, &Resources::standardShader, &(Resources::darkGreyMaterial), parentTransform* t);
	Graphics::RenderScene(&Resources::aventadorBodyGlow, &Resources::standardShader, &Resources::emmisiveBlueMaterial, parentTransform*t);
	Graphics::RenderScene(&Resources::aventadorUnder, &Resources::standardShader, &Resources::pureBlackMaterial, parentTransform*t);

	wheel1.get()->update(parentTransform*t);
	wheel2.get()->update(parentTransform*t);
	wheel0.get()->update(parentTransform*t);
	wheel3.get()->update(parentTransform*t);
}

void AventadorWheel::update0(glm::mat4 parentTransform) {
	transform = rotate(transform, rotateSpeed, vec3(1.0f, 0.0f, 0.0f));
	Light::renderShadowMap(&Resources::aventadorWheel, parentTransform*transform);
	Light::renderShadowMap(&Resources::aventadorWheelGlow, parentTransform*transform);
}

void AventadorWheel::update(glm::mat4 parentTransform) {
	Graphics::RenderScene(&Resources::aventadorWheel, &Resources::standardShader, &Resources::darkGreyMaterial, parentTransform*transform);
	Graphics::RenderScene(&Resources::aventadorWheelGlow, &Resources::standardShader, &Resources::emmisiveBlueMaterial, parentTransform*transform);
}

namespace Game {
	vector<unique_ptr<Entity>> entities;

	// we can customize this function as much as we want for now for debugging
	void init() {
		entities.push_back(unique_ptr<Aventador>(new Aventador));
		//	entities.push_back(unique_ptr<Cube>(new Cube));
		entities.push_back(unique_ptr<CenteredCube>(new CenteredCube(vec3(0, 3, 0))));
		entities.push_back(unique_ptr<Plane>(new Plane));
	}

	void update() {
		glfwPollEvents();

		for (int i = 0; i < entities.size(); i++) {
			entities[i].get()->update0(mat4(1));
		}
		for (int i = 0; i < entities.size(); i++) {
			entities[i].get()->update(mat4(1));
		}

		if (Keyboard::keyPressed(GLFW_KEY_Q))cout << "q pressed\n";
		if (Keyboard::keyReleased(GLFW_KEY_Q))cout << "q released\n";
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
	Light::renderShadowMap(&Resources::plane, parentTransform*transform);
}

void Plane::update(glm::mat4 parentTransform) {
	Graphics::RenderScene(&Resources::plane, &Resources::standardShader, &Resources::defaultMaterial, parentTransform*transform);
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
	PxBoxGeometry geometry(dimensions);
	actor = PxCreateDynamic(*PhysicsManager::mPhysics, t, geometry, *PhysicsManager::mPhysics->createMaterial(0.1f, 0.1f, 0.5f), PxReal(1.0f));
	actor->setAngularDamping(0.1);
	PhysicsManager::mScene->addActor(*actor);
}

void CenteredCube::update0(glm::mat4 parentTransform) {
	if (Keyboard::keyDown(GLFW_KEY_W)) {
		actor->addForce(PxVec3(0, 0, 10));
	}
	if (Keyboard::keyDown(GLFW_KEY_A)) {
		actor->addForce(PxVec3(10, 0, 0));
	}
	if (Keyboard::keyDown(GLFW_KEY_S)) {
		actor->addForce(PxVec3(00, 0, -10));
	}
	if (Keyboard::keyDown(GLFW_KEY_D)) {
		actor->addForce(PxVec3(-10, 0, 0));
	}

	glm::mat4 m = glm::translate(glm::mat4(1), glm::vec3(actor->getGlobalPose().p.x, actor->getGlobalPose().p.y, actor->getGlobalPose().p.z));
	PxReal a; PxVec3 b;  actor->getGlobalPose().q.toRadiansAndUnitAxis(a, b); m = glm::rotate(m, (float)a, glm::vec3(b.x, b.y, b.z));
	transform = m;

	Light::renderShadowMap(&Resources::centeredCube, transform);
}

void CenteredCube::update(glm::mat4 parentTransform) {
	Graphics::RenderScene(&Resources::centeredCube, &Resources::standardShader, &Resources::defaultMaterial, transform);
}