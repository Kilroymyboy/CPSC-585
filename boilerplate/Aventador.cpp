#include "Aventador.h"

using namespace std;
using namespace glm;
using namespace physx;

Aventador::Aventador() {
	wheel.resize(4);
	wheelPos.resize(4);
	wheelHit.resize(4);
	wheelHitInfo.resize(4);

	wheelAngle = 0;
	maxWheelDist = 0.53;

	wheelPos[0] = vec3(-.851f, .331f, 1.282f);
	wheelPos[1] = vec3(.851f, .331f, 1.282f);
	wheelPos[2] = vec3(.858f, .356f, -1.427f);
	wheelPos[3] = vec3(-.858f, .356f, -1.427f);
	modelDisplacement = vec3(0, -0.55, 0);

	wheel[1] = std::unique_ptr<AventadorWheel>(new AventadorWheel);
	wheel[1].get()->transform = translate(mat4(1), wheelPos[1]);
	wheel[2] = std::unique_ptr<AventadorWheel>(new AventadorWheel);
	wheel[2].get()->transform = scale(translate(mat4(1), wheelPos[2]), vec3(1.07f, 1.07f, 1.07f));

	wheel[1].get()->rotateInverse = 1;
	wheel[2].get()->rotateInverse = 1;

	wheel[0] = std::unique_ptr<AventadorWheel>(new AventadorWheel);
	wheel[0].get()->transform = rotate(translate(mat4(1), wheelPos[0]), (float)PI, vec3(0, 1, 0));
	wheel[3] = std::unique_ptr<AventadorWheel>(new AventadorWheel);
	wheel[3].get()->transform = scale(rotate(translate(mat4(1), wheelPos[3]), (float)PI, vec3(0, 1, 0)), vec3(1.07f, 1.07f, 1.07f));

	wheel[0].get()->rotateInverse = -1;
	wheel[3].get()->rotateInverse = -1;

	PxTransform t(PxVec3(0, 5, 0), PxQuat::createIdentity());
	PxVec3 dimensions(1, 0.45, 2.5);

	actor = PhysicsManager::createDynamic(t, dimensions);
	actor->setMass(5.5);
	actor->setAngularDamping(0.8);
	actor->setLinearDamping(0.5);
}

void Aventador::update0(glm::mat4 parentTransform) {
	if (Keyboard::keyPressed(GLFW_KEY_SPACE)) {
		actor->addForce(PxVec3(0, 35, 0), PxForceMode::eIMPULSE);
	}

	if (Keyboard::keyPressed(GLFW_KEY_A)) {
		PxRigidBodyExt::addLocalForceAtLocalPos(*actor, PxVec3(60, 0, 0), PxVec3(0, 0, 0), PxForceMode::eIMPULSE);
	}
	if (Keyboard::keyPressed(GLFW_KEY_D)) {
		PxRigidBodyExt::addLocalForceAtLocalPos(*actor, PxVec3(-60, 0, 0), PxVec3(0, 0, 0), PxForceMode::eIMPULSE);
	}
	if (Keyboard::keyPressed(GLFW_KEY_Z)) {
		actor->addTorque(PxVec3(0, 2650, 0));
	}
	if (Keyboard::keyPressed(GLFW_KEY_C)) {
		actor->addTorque(PxVec3(0, -2650, 0));
	}

	inverseRotation = inverse(mat3(transform));

	vec3 pos(actor->getGlobalPose().p.x, actor->getGlobalPose().p.y, actor->getGlobalPose().p.z);
	glm::mat4 m = glm::translate(glm::mat4(1), pos);
	PxReal a; PxVec3 b;  actor->getGlobalPose().q.toRadiansAndUnitAxis(a, b); m = glm::rotate(m, (float)a, glm::vec3(b.x, b.y, b.z));
	transform = m;

	tempTransform = translate(transform, modelDisplacement);

	float positionTightness = .2, targetTightness = .5;
	Viewport::position = mix(Viewport::position, vec3(transform* vec4(0, 1.25f, -5.5f, 1)), positionTightness);
	Viewport::target = mix(Viewport::target, vec3(transform* vec4(0, 1.25f, 0, 1)), targetTightness);
	if (Keyboard::keyDown(GLFW_KEY_Q)) {
		Viewport::position = transform* vec4(5.5f, 1.25f, 0.0f, 1);
	}
	else	if (Keyboard::keyDown(GLFW_KEY_E)) {
		Viewport::position = transform* vec4(-5.5f, 1.25f, 0.0f, 1);
	}

	raycastWheels();
	updateSuspension();
	updateFriction();
	updateSteering();

	Light::position = pos + vec3(3, 5, 4);
	Light::target = pos;

	Light::renderShadowMap(&Resources::aventadorBody, tempTransform);
	Light::renderShadowMap(&Resources::aventadorBodyGlow, tempTransform);
	Light::renderShadowMap(&Resources::aventadorUnder, tempTransform);

	for (int i = 0; i < wheel.size(); i++) {
		wheel[i].get()->update0(tempTransform);
	}
}

void Aventador::update(glm::mat4 parentTransform) {
	Graphics::RenderScene(&Resources::aventadorBody, &Resources::standardShader, &(Resources::darkGreyMaterial), tempTransform);
	Graphics::RenderScene(&Resources::aventadorBodyGlow, &Resources::standardShader, &Resources::emmisiveBlueMaterial, tempTransform);
	Graphics::RenderScene(&Resources::aventadorUnder, &Resources::standardShader, &Resources::pureBlackMaterial, tempTransform);

	for (int i = 0; i < wheel.size(); i++) {
		wheel[i].get()->update(tempTransform);
	}
}

void Aventador::raycastWheels() {
	PxRaycastBuffer hit;
	PxHitFlags hitFlags = PxHitFlag::ePOSITION | PxHitFlag::eNORMAL | PxHitFlag::eDISTANCE;
	PxQueryFilterData filterData(PxQueryFlag::eSTATIC);

	for (int i = 0; i < 4; i++) {
		vec3 wheelP(transform*vec4(wheelPos[i] - vec3(0, .45, 0), 1));
		if (PhysicsManager::mScene->raycast(
			Util::g2p(wheelP),
			Util::g2p(mat3(transform)*vec3(0, -1, 0)), maxWheelDist,
			hit, hitFlags, filterData)) {
			wheelHit[i] = 1;
			wheelHitInfo[i] = hit;
		}
		else {
			wheelHit[i] = 0;
		}
	}
}

void Aventador::updateSuspension() {
	for (int i = 0; i < 4; i++) {
		if (wheelHit[i]) {
			PxRigidBodyExt::addForceAtLocalPos(*actor,
				PxVec3(0, springForce * max(0, (maxWheelDist - wheelHitInfo[i].block.distance)), 0),
				Util::g2p(wheelPos[i]), PxForceMode::eFORCE);
			wheel[i].get()->height = (-wheelHitInfo[i].block.distance + wheelPos[i].y) + 0.09;

			PxVec3 wspeed = PxRigidBodyExt::getLocalVelocityAtLocalPos(*actor, Util::g2p(wheelPos[i]));
			PxRigidBodyExt::addForceAtLocalPos(*actor,
				PxVec3(0, damperForce * -wspeed.y, 0),
				Util::g2p(wheelPos[i]), PxForceMode::eFORCE);

			wheel[i].get()->rotateSpeed = (inverseRotation*Util::p2g(wspeed)).z *0.035;
		}
		else {
			wheel[i].get()->height = (-maxWheelDist + wheelPos[i].y) + 0.09;
			wheel[i].get()->rotateSpeed *= 0.95;
		}
	}
}

void Aventador::updateFriction() {
	for (int i = 0; i < 2; i++) {
		if (wheelHit[i]) {
			if (Keyboard::keyDown(GLFW_KEY_UP)) {
				PxRigidBodyExt::addLocalForceAtLocalPos(*actor,
					PxVec3(sin(wheelAngle) * 25, 0, cos(wheelAngle) * 25), Util::g2p(wheelPos[i]), PxForceMode::eFORCE);
			}
			else if (Keyboard::keyDown(GLFW_KEY_DOWN)) {
				PxRigidBodyExt::addLocalForceAtLocalPos(*actor,
					PxVec3(sin(wheelAngle) * -25, 0, cos(wheelAngle) * -25), Util::g2p(wheelPos[i]), PxForceMode::eFORCE);
			}

			PxVec3 wspeed = PxRigidBodyExt::getVelocityAtPos(*actor, Util::g2p(transform*vec4(wheelPos[i], 1)));

			PxRigidBodyExt::addForceAtPos(*actor, -wspeed,
				Util::g2p(transform*vec4(wheelPos[i], 1)), PxForceMode::eFORCE);
		}
	}
	for (int i = 2; i < 4; i++) {
		if (wheelHit[i]) {

			if (Keyboard::keyDown(GLFW_KEY_UP)) {
				PxRigidBodyExt::addLocalForceAtLocalPos(*actor,
					PxVec3(0, 0, 25), Util::g2p(wheelPos[i]), PxForceMode::eFORCE);
			}
			else if (Keyboard::keyDown(GLFW_KEY_DOWN)) {
				PxRigidBodyExt::addLocalForceAtLocalPos(*actor,
					PxVec3(0, 0, -25), Util::g2p(wheelPos[i]), PxForceMode::eFORCE);
			}
			if (!Keyboard::keyDown(GLFW_KEY_UP) && !Keyboard::keyDown(GLFW_KEY_DOWN)) {
			//	PxRigidBodyExt::addLocalForceAtLocalPos(*actor, PxVec3(0, 0, sign(wspeed.z)*-10),
			//		Util::g2p(wheelPos[i]), PxForceMode::eFORCE);
			}

			PxVec3 wspeed = PxRigidBodyExt::getVelocityAtPos(*actor, Util::g2p(transform*vec4(wheelPos[i], 1)));

			PxRigidBodyExt::addForceAtPos(*actor, -wspeed,
				Util::g2p(transform*vec4(wheelPos[i], 1)), PxForceMode::eFORCE);
		}
	}

	{
		PxVec3 v = actor->getLinearVelocity();
		v.y = 0;
		v = Util::g2p(inverseRotation*Util::p2g(v));
		if (v.z > 0.05) {
		//	actor->addTorque(PxVec3(0, wheelAngle * min(2, abs(v.z)) * sign(v.z), 0));
		}
	}
}

void Aventador::updateSteering() {
	if (Keyboard::keyDown(GLFW_KEY_LEFT)) {
		wheelAngle += 0.035;
	}
	if (Keyboard::keyDown(GLFW_KEY_RIGHT)) {
		wheelAngle -= 0.035;
	}
	if (!(Keyboard::keyDown(GLFW_KEY_LEFT) || Keyboard::keyDown(GLFW_KEY_RIGHT))) {
		wheelAngle *= 0.9;
	}
	wheelAngle = min(max(wheelAngle, -maxWheelAngle), maxWheelAngle);
	wheel[0].get()->facingAngle = wheelAngle;
	wheel[1].get()->facingAngle = wheelAngle;
}

void AventadorWheel::update0(glm::mat4 parentTransform) {
	rotation += rotateSpeed*rotateInverse;
	if (rotation > 2 * PI)rotation = 0;
	if (rotation < -2 * PI)rotation = 0;
	tempTransform = translate(transform, vec3(0.0f, height, 0.0f));
	tempTransform = rotate(tempTransform, facingAngle, vec3(0.0f, 1.0f, 0.0f));
	tempTransform = rotate(tempTransform, rotation, vec3(1.0f, 0.0f, 0.0f));
	Light::renderShadowMap(&Resources::aventadorWheel, parentTransform*tempTransform);
	Light::renderShadowMap(&Resources::aventadorWheelGlow, parentTransform*tempTransform);
}

void AventadorWheel::update(glm::mat4 parentTransform) {
	Graphics::RenderScene(&Resources::aventadorWheel, &Resources::standardShader, &Resources::darkGreyMaterial, parentTransform*tempTransform);
	Graphics::RenderScene(&Resources::aventadorWheelGlow, &Resources::standardShader, &Resources::emmisiveBlueMaterial, parentTransform*tempTransform);
}
