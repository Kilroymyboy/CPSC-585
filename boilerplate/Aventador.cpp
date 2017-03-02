#include "Aventador.h"
#include <glm\gtx\projection.hpp>

using namespace std;
using namespace glm;
using namespace physx;

Aventador::Aventador(int id) {
	aventadorId = id;

	wheel.resize(4);
	wheelPos.resize(4);
	wheelHit.resize(4);
	wheelHitInfo.resize(4);

	wheelAngle = 0;
	tireHeat.assign(4, 0);

	wheelPos[0] = vec3(-.851f, .331f, 1.282f);
	wheelPos[1] = vec3(.851f, .331f, 1.282f);
	wheelPos[2] = vec3(.858f, .356f, -1.427f);
	wheelPos[3] = vec3(-.858f, .356f, -1.427f);
	modelDisplacement = vec3(0, -aventadorData.dimensionHeight, 0);

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
	PxVec3 dimensions(1, aventadorData.dimensionHeight, 2.5);

	actor = PhysicsManager::createDynamic(t, dimensions);
	actor->setMass(5.5);
	actor->setAngularDamping(0.8);
	actor->setLinearDamping(0.5);

	PhysicsManager::attachSimulationShape(actor, dimensions,200);
	PhysicsManager::setContactFilter(actor, FilterGroup::eAventador, FilterGroup::eAventador | FilterGroup::ePowerUp);
}

void Aventador::update(glm::mat4 parentTransform) {
	vec3 pos = Util::p2g(actor->getGlobalPose().p);
	glm::mat4 m = glm::translate(glm::mat4(1), pos);
	PxReal a; PxVec3 b;  actor->getGlobalPose().q.toRadiansAndUnitAxis(a, b); m = glm::rotate(m, (float)a, glm::vec3(b.x, b.y, b.z));
	transform = m;

	inverseRotation = inverse(mat3(transform));

	tempTransform = translate(transform, modelDisplacement);

	raycastWheels();
	updateSuspension();
	updateFriction();
	updateSteering();
	updateTopSpeed();
	updateDrift();
	updateBraking();

	updateLightCamera();


	for (int i = 0; i < wheel.size(); i++) {
		wheel[i].get()->update(tempTransform);
	}
}

void Aventador::updateLightCamera() {
	vec3 pos = Util::p2g(actor->getGlobalPose().p);

	float positionTightness = .3, targetTightness = .9;

	Viewport::position[aventadorId] = mix(Viewport::position[aventadorId], vec3(transform* vec4(0, 1.25f, -5.5f, 1)), positionTightness);
	Viewport::target[aventadorId] = mix(Viewport::target[aventadorId], vec3(transform* vec4(0, 1.25f, 0, 1)), targetTightness);
	if (Keyboard::keyDown(GLFW_KEY_Q)) {
		Viewport::position[aventadorId] = transform* vec4(5.5f, 1.25f, 0.0f, 1);
	}
	else	if (Keyboard::keyDown(GLFW_KEY_E)) {
		Viewport::position[aventadorId] = transform* vec4(-5.5f, 1.25f, 0.0f, 1);
	}

	Light::position[aventadorId] = pos + vec3(3, 5, 4);
	Light::target[aventadorId] = pos;

}

void Aventador::renderShadowMap(glm::mat4 parentTransform) {
	Light::renderShadowMap(&Resources::aventadorBody, tempTransform);
	Light::renderShadowMap(&Resources::aventadorBodyGlow, tempTransform);
	Light::renderShadowMap(&Resources::aventadorUnder, tempTransform);

	for (int i = 0; i < wheel.size(); i++) {
		wheel[i].get()->renderShadowMap(tempTransform);
	}
}

void Aventador::render(glm::mat4 parentTransform) {
	Graphics::Render(&Resources::aventadorBody, &(Resources::darkGreyMaterial), tempTransform);
	Graphics::Render(&Resources::aventadorBodyGlow, &Resources::emmisiveBlueMaterial, tempTransform);
	Graphics::Render(&Resources::aventadorUnder, &Resources::pureBlackMaterial, tempTransform);

	for (int i = 0; i < wheel.size(); i++) {
		wheel[i].get()->render(tempTransform);
	}
}

void Aventador::raycastWheels() {
	PxRaycastBuffer hit;
	PxHitFlags hitFlags = PxHitFlag::ePOSITION | PxHitFlag::eNORMAL | PxHitFlag::eDISTANCE;
	PxQueryFilterData filterData(PxQueryFlag::eSTATIC);

	for (int i = 0; i < 4; i++) {
		vec3 wheelP(transform*vec4(wheelPos[i] - vec3(0, aventadorData.dimensionHeight, 0), 1));
		if (PhysicsManager::mScene->raycast(
			Util::g2p(wheelP),
			Util::g2p(mat3(transform)*vec3(0, -1, 0)), aventadorData.maxWheelDist,
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
				PxVec3(0, aventadorData.springForce * max(0, (aventadorData.maxWheelDist - wheelHitInfo[i].block.distance)), 0),
				Util::g2p(wheelPos[i] - vec3(0, aventadorData.dimensionHeight, 0)), PxForceMode::eFORCE);
			wheel[i].get()->height = (-wheelHitInfo[i].block.distance + wheelPos[i].y);

			PxVec3 wspeed = PxRigidBodyExt::getLocalVelocityAtLocalPos(*actor, Util::g2p(wheelPos[i]));
			PxRigidBodyExt::addForceAtLocalPos(*actor,
				PxVec3(0, aventadorData.damperForce * -wspeed.y, 0),
				Util::g2p(wheelPos[i] - vec3(0, aventadorData.dimensionHeight, 0)), PxForceMode::eFORCE);

			wheel[i].get()->rotateSpeed = (inverseRotation*Util::p2g(wspeed)).z *0.035;
		}
		else {
			wheel[i].get()->height = (-aventadorData.maxWheelDist + wheelPos[i].y);
			wheel[i].get()->rotateSpeed *= 0.95;
		}
	}
}

void Aventador::updateFriction() {
	for (int i = 0; i < 4; i++) {
		float wheelA = i < 2 ? wheelAngle : 0;
		vec3 wheeld(sin(wheelA), 0, cos(wheelA));
		wheeld = mat3(transform)*wheeld;
		if (wheelHit[i]) {
			if (Keyboard::keyDown(aventadorId ? GLFW_KEY_UP : GLFW_KEY_W)) {
				PxRigidBodyExt::addLocalForceAtLocalPos(*actor,
					PxVec3(sin(wheelA) * aventadorData.force, 0, cos(wheelA) *  aventadorData.force),
					Util::g2p(wheelPos[i] - vec3(0, aventadorData.dimensionHeight, 0)), PxForceMode::eFORCE);
			}
			else if (Keyboard::keyDown(aventadorId ? GLFW_KEY_DOWN : GLFW_KEY_S)) {
				PxRigidBodyExt::addLocalForceAtLocalPos(*actor,
					PxVec3(sin(wheelA) * -aventadorData.force, 0, cos(wheelA) * -aventadorData.force),
					Util::g2p(wheelPos[i] - vec3(0, aventadorData.dimensionHeight, 0)), PxForceMode::eFORCE);
			}

			PxVec3 wspeed = PxRigidBodyExt::getVelocityAtPos(*actor, Util::g2p(transform*vec4(wheelPos[i], 1)));
			wspeed.y = 0;
			vec3 forwardv = proj(Util::p2g(wspeed), wheeld); forwardv.y = 0;
			vec3 frictionv = Util::p2g(wspeed) - forwardv;
			vec3 frictiond = -normalize(frictionv); frictiond.y = 0;
			PxRigidBodyExt::addForceAtPos(*actor, Util::g2p(frictiond / (1 + tireHeat[i]))*min(wspeed.magnitude() * aventadorData.wheelSideFriction, aventadorData.wheelSideMaxFriction),
				Util::g2p(transform*vec4(wheelPos[i] - vec3(0, aventadorData.dimensionHeight, 0), 1)), PxForceMode::eFORCE);
			PxRigidBodyExt::addForceAtPos(*actor, Util::g2p(-normalize(forwardv)*brakeForce),
				Util::g2p(transform*vec4(wheelPos[i] - vec3(0, aventadorData.dimensionHeight, 0), 1)), PxForceMode::eFORCE);
			tireHeat[i] += length(frictionv) *aventadorData.tireHeatIncrease[i];
		}
	}
}

void Aventador::updateSteering() {
	if (Keyboard::keyDown(aventadorId ? GLFW_KEY_LEFT : GLFW_KEY_A)) {
		wheelAngle += aventadorData.wheelTurnRate;
	}
	if (Keyboard::keyDown(aventadorId ? GLFW_KEY_RIGHT : GLFW_KEY_D)) {
		wheelAngle -= aventadorData.wheelTurnRate;
	}
	if (!(Keyboard::keyDown(aventadorId ? GLFW_KEY_LEFT : GLFW_KEY_A)
		|| Keyboard::keyDown(aventadorId ? GLFW_KEY_RIGHT : GLFW_KEY_D))) {
		wheelAngle *= aventadorData.wheelReurnRate;
	}
	wheelAngle = min(max(wheelAngle, -aventadorData.maxWheelAngle), aventadorData.maxWheelAngle);
	wheel[0].get()->facingAngle = wheelAngle;
	wheel[1].get()->facingAngle = wheelAngle;
}

void Aventador::updateTopSpeed() {
	PxVec3 v = -actor->getLinearVelocity();
	v.y = 0;
	actor->addForce(v*aventadorData.topSpeedFriction);
}

void Aventador::updateDrift() {
	if (Keyboard::keyDown(aventadorId ? GLFW_KEY_RIGHT_CONTROL : GLFW_KEY_LEFT_CONTROL)) {
		// manually add heat to rear tires
		PxVec3 v = actor->getLinearVelocity();
		tireHeat[2] += aventadorData.manualTireHeatIncrease*v.magnitude();
		tireHeat[3] += aventadorData.manualTireHeatIncrease*v.magnitude();
	}

	if (Keyboard::keyDown(aventadorId ? GLFW_KEY_UP : GLFW_KEY_W))
		for (int i = 0; i < tireHeat.size(); i++)tireHeat[i] *= aventadorData.tireHeatDecrease;
	else
		for (int i = 0; i < tireHeat.size(); i++)tireHeat[i] *= aventadorData.tireHeatFastDecrease;
}

void Aventador::updateBraking() {
	if (Keyboard::keyDown(aventadorId ? GLFW_KEY_RIGHT_CONTROL : GLFW_KEY_LEFT_CONTROL)) {
		brakeForce = min(brakeForce + aventadorData.brakeSpeed, aventadorData.maxBrakeForce);
	}
	else {
		brakeForce = 0;
	}
}

void AventadorWheel::update(glm::mat4 parentTransform) {
	rotation += rotateSpeed*rotateInverse;
	rotation = rotation - 2 * PI*((int)(rotation / (2 * PI)));
	tempTransform = translate(transform, vec3(0.0f, height, 0.0f));
	tempTransform = rotate(tempTransform, facingAngle, vec3(0.0f, 1.0f, 0.0f));
	tempTransform = rotate(tempTransform, rotation, vec3(1.0f, 0.0f, 0.0f));
}

void AventadorWheel::renderShadowMap(glm::mat4 parentTransform) {
	Light::renderShadowMap(&Resources::aventadorWheel, parentTransform*tempTransform);
	Light::renderShadowMap(&Resources::aventadorWheelGlow, parentTransform*tempTransform);
}

void AventadorWheel::render(glm::mat4 parentTransform) {
	Graphics::Render(&Resources::aventadorWheel, &Resources::darkGreyMaterial, parentTransform*tempTransform);
	Graphics::Render(&Resources::aventadorWheelGlow, &Resources::emmisiveBlueMaterial, parentTransform*tempTransform);
}
