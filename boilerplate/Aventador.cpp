#include "Aventador.h"

using namespace std;
using namespace glm;
using namespace physx;

Aventador::Aventador() {
	wheel.resize(4);
	wheelPos.resize(4);

	wheelPos[0] = vec3(-.851f, .331f, 1.282f);
	wheelPos[1] = vec3(.851f, .331f, 1.282f);
	wheelPos[2] = vec3(.858f, .356f, -1.427f);
	wheelPos[3] = vec3(-.858f, .356f, -1.427f);
	modelDisplacement = vec3(0, -0.55, 0);

	wheel[1] = std::unique_ptr<AventadorWheel>(new AventadorWheel);
	wheel[1].get()->transform = translate(mat4(1), wheelPos[1]);
	wheel[2] = std::unique_ptr<AventadorWheel>(new AventadorWheel);
	wheel[2].get()->transform = scale(translate(mat4(1), wheelPos[2]), vec3(1.07f, 1.07f, 1.07f));

	wheel[1].get()->rotateSpeed = .1f;
	wheel[2].get()->rotateSpeed = .1f;

	wheel[0] = std::unique_ptr<AventadorWheel>(new AventadorWheel);
	wheel[0].get()->transform = rotate(translate(mat4(1), wheelPos[0]), (float)PI, vec3(0, 1, 0));
	wheel[3] = std::unique_ptr<AventadorWheel>(new AventadorWheel);
	wheel[3].get()->transform = scale(rotate(translate(mat4(1), wheelPos[3]), (float)PI, vec3(0, 1, 0)), vec3(1.07f, 1.07f, 1.07f));

	wheel[0].get()->rotateSpeed = -.1f;
	wheel[3].get()->rotateSpeed = -.1f;

	PxTransform t(PxVec3(0, 5, 0), PxQuat::createIdentity());
	PxVec3 dimensions(1, 0.45, 2.5);

	actor = PhysicsManager::createDynamic(t, dimensions);
	actor->setMass(5.5);
}

void Aventador::update0(glm::mat4 parentTransform) {
	if (Keyboard::keyDown(GLFW_KEY_W) || Keyboard::keyDown(GLFW_KEY_UP)) {
		PxRigidBodyExt::addLocalForceAtLocalPos(*actor, PxVec3(0, 0, 100), PxVec3(0));
	}
	if (Keyboard::keyDown(GLFW_KEY_A) || Keyboard::keyDown(GLFW_KEY_LEFT)) {
		PxRigidBodyExt::addLocalForceAtLocalPos(*actor, PxVec3(20, 0, 0), PxVec3(0, 0, 10));
	}
	if (Keyboard::keyDown(GLFW_KEY_S) || Keyboard::keyDown(GLFW_KEY_DOWN)) {
		PxRigidBodyExt::addLocalForceAtLocalPos(*actor, PxVec3(0, 0, -100), PxVec3(0));
	}
	if (Keyboard::keyDown(GLFW_KEY_D) || Keyboard::keyDown(GLFW_KEY_RIGHT)) {
		PxRigidBodyExt::addLocalForceAtLocalPos(*actor, PxVec3(20, 0, 0), PxVec3(0, 0, -10));
	}

	vec3 pos(actor->getGlobalPose().p.x, actor->getGlobalPose().p.y, actor->getGlobalPose().p.z);
	glm::mat4 m = glm::translate(glm::mat4(1), pos);
	PxReal a; PxVec3 b;  actor->getGlobalPose().q.toRadiansAndUnitAxis(a, b); m = glm::rotate(m, (float)a, glm::vec3(b.x, b.y, b.z));
	transform = m;

	mat4 t = translate(transform, modelDisplacement);

	float positionTightness = .2, targetTightness = .5;
	Viewport::position = mix(Viewport::position, vec3(transform* vec4(0, 1.25f, -5.5f, 1)), positionTightness);
	Viewport::target = mix(Viewport::target, vec3(transform* vec4(0, 1.25f, 0, 1)), targetTightness);
	if (Keyboard::keyDown(GLFW_KEY_Q)) {
		Viewport::position = transform* vec4(5.5f, 1.25f, 0.0f, 1);
	}
	else	if (Keyboard::keyDown(GLFW_KEY_E)) {
		Viewport::position = transform* vec4(-5.5f, 1.25f, 0.0f, 1);
	}

	updateSuspension();

	Light::position = pos + vec3(3, 5, 4);
	Light::target = pos;

	Light::renderShadowMap(&Resources::aventadorBody, t);
	Light::renderShadowMap(&Resources::aventadorBodyGlow, t);
	Light::renderShadowMap(&Resources::aventadorUnder, t);

	for (int i = 0; i < wheel.size(); i++) {
		//	wheel[i].get()->update0(t);
	}
}

void Aventador::update(glm::mat4 parentTransform) {
	mat4 t = translate(transform, modelDisplacement);

	Graphics::RenderScene(&Resources::aventadorBody, &Resources::standardShader, &(Resources::darkGreyMaterial), t);
	Graphics::RenderScene(&Resources::aventadorBodyGlow, &Resources::standardShader, &Resources::emmisiveBlueMaterial, t);
	Graphics::RenderScene(&Resources::aventadorUnder, &Resources::standardShader, &Resources::pureBlackMaterial, t);

	for (int i = 0; i < wheel.size(); i++) {
		//	wheel[i].get()->update(t);
	}
}

void Aventador::updateSuspension() {
	PxRaycastBuffer hit;
	PxHitFlags hitFlags = PxHitFlag::ePOSITION | PxHitFlag::eNORMAL | PxHitFlag::eDISTANCE;
	PxQueryFilterData filterData(PxQueryFlag::eSTATIC);

	for (int i = 0; i < 4; i++)
		if (PhysicsManager::mScene->raycast(
			Util::g2p(vec3(transform*vec4(wheelPos[i], 1))),
			Util::g2p(mat3(transform)*vec3(0, -1, 0)), 1,
			hit, hitFlags, filterData)) {
			PxRigidBodyExt::addForceAtLocalPos(*actor,
				PxVec3(0, -28.5 * min(0, (wheelPos[i].y - hit.block.distance)), 0),
				Util::g2p(wheelPos[i]));
		}
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
