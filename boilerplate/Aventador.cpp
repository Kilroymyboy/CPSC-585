#include "Aventador.h"
#include "Game.h"
#include "PowerUp.h"
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
	PxVec3 dimensions(aventadorData.dimensionWidth, aventadorData.dimensionHeight, aventadorData.dimensionLength);

	actor = PhysicsManager::createDynamic(t, dimensions);
	actor->setMass(5.5);
	actor->setLinearDamping(0.5);
	actor->userData = (void*)(ContactModFlags::eIGNORE_CONTACT);

	PhysicsManager::attachSimulationShape(actor, dimensions, 200);
	PhysicsManager::setContactFilter(actor, FilterGroup::eAventador, FilterGroup::eAventador | FilterGroup::ePowerUp);

	if (aventadorId == 0) {
		actor->setGlobalPose(PxTransform(0, 0, 15.0), true);
		aventadorData.isFront = true;
		aventadorData.force = forceFront;
		if (Game::vs_ai) {	//If the player is versing AI
			AiManager::aiInit(aventadorData.isAI);
		}
	}
	else {
		aventadorData.isFront = false;
		aventadorData.force = forceBack;
	}

	fullHealthColor = vec3(1.8, 4.8, 12.6);
	noHealthColor = vec3(5.2, .8, .8);

	material = Graphics::Material(vec3(1));
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
	updateColour();
	updateCurrentPowerUp(bubbleType);

	if (!aventadorData.isFront)
		updateFuel();


	updateLightCamera();
	usePowerUp();

	for (int i = 0; i < wheel.size(); i++) {
		wheel[i].get()->material.color = material.color;
		wheel[i].get()->material.emmisiveColor = material.emmisiveColor;
		wheel[i].get()->update(tempTransform);
	}
	if (hasPowerUp()) {
		currentPowerUp[0].get()->update(tempTransform);
	}
}

void Aventador::updateLightCamera() {
	vec3 pos = Util::p2g(actor->getGlobalPose().p);

	vec3 targetViewportPos = vec3(transform* vec4(0, 1.25f, -5.5f, 1));
	targetViewportPos.y = 1.3;
	Viewport::position[aventadorId] = mix(Viewport::position[aventadorId], targetViewportPos, positionTightness);
	Viewport::target[aventadorId] = mix(Viewport::target[aventadorId], vec3(transform* vec4(0, 1.25f, 0, 1)), targetTightness);
	if (Keyboard::keyDown(GLFW_KEY_Q)) {
		Viewport::position[aventadorId] = transform* vec4(5.5f, 1.25f, 0.0f, 1);
	}
	else if (Keyboard::keyDown(GLFW_KEY_E)) {
		Viewport::position[aventadorId] = transform* vec4(-5.5f, 1.25f, 0.0f, 1);
	}

	Light::position[aventadorId] = pos + vec3(3, 5, 4);
	Light::target[aventadorId] = pos;

}

void Aventador::renderShadowMap(glm::mat4 parentTransform) {
	Light::renderShadowMapInstanced(&Resources::aventadorBody, tempTransform);
	Light::renderShadowMapInstanced(&Resources::aventadorBodyGlow, tempTransform);
	Light::renderShadowMapInstanced(&Resources::aventadorUnder, tempTransform);

	for (int i = 0; i < wheel.size(); i++) {
		wheel[i].get()->renderShadowMap(tempTransform);
	}
}

void Aventador::updateColour() {
	if (max(aventadorData.fuel, 0) * 4 < aventadorData.tankSize) {
		if (Time::time > nextFlashTime) {
			nextFlashTime = max(flashCooldown*(double)max(aventadorData.fuel, 0), 0.05) / aventadorData.tankSize + Time::time;
			if (material.emmisiveColor == vec3(0)) {
				material.emmisiveColor = mix(noHealthColor, fullHealthColor, (double)max(aventadorData.fuel, 0) / aventadorData.tankSize);
			}
			else {
				material.emmisiveColor = vec3(0);
			}
		}
	}
	else {
		material.emmisiveColor = mix(noHealthColor, fullHealthColor, (double)max(aventadorData.fuel, 0) / aventadorData.tankSize);
	}
	if (aventadorData.fuel < 0) {
		material.emmisiveColor = vec3(0);
	}
}

void Aventador::render(glm::mat4 parentTransform) {
	Graphics::RenderInstanced(&Resources::aventadorBody, &Resources::darkGreyMaterial, tempTransform);
	Graphics::RenderInstanced(&Resources::aventadorBodyGlow, &material, tempTransform);
	Graphics::RenderInstanced(&Resources::aventadorUnder, &Resources::pureBlackMaterial, tempTransform);

	for (int i = 0; i < wheel.size(); i++) {
		wheel[i].get()->render(tempTransform);
	}
	if (hasPowerUp()) {
		currentPowerUp[0].get()->render(tempTransform);
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
			if (aventadorData.isAI) {
				PxRigidBodyExt::addLocalForceAtLocalPos(*actor,
					PxVec3(sin(wheelA) * aventadorData.force, 0, cos(wheelA) *  aventadorData.force),
					Util::g2p(wheelPos[i] - vec3(0, aventadorData.dimensionHeight, 0)), PxForceMode::eFORCE);
			}
			else {
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
				if (aventadorId == 0) {
					float amount = controller1.RightTrigger();
					PxRigidBodyExt::addLocalForceAtLocalPos(*actor,
						PxVec3(sin(wheelA) * (aventadorData.force*amount), 0, cos(wheelA) * (aventadorData.force*amount)),
						Util::g2p(wheelPos[i] - vec3(0, aventadorData.dimensionHeight, 0)), PxForceMode::eFORCE);
				}
				if (aventadorId == 1) {
					float amount = controller2.RightTrigger();
					PxRigidBodyExt::addLocalForceAtLocalPos(*actor,
						PxVec3(sin(wheelA) * (aventadorData.force*amount), 0, cos(wheelA) * (aventadorData.force*amount)),
						Util::g2p(wheelPos[i] - vec3(0, aventadorData.dimensionHeight, 0)), PxForceMode::eFORCE);
				}
				if (aventadorId == 0) {
					float amount = controller1.LeftTrigger();
					PxRigidBodyExt::addLocalForceAtLocalPos(*actor,
						PxVec3(sin(wheelA) * (-aventadorData.force*amount), 0, cos(wheelA) * (-aventadorData.force*amount)),
						Util::g2p(wheelPos[i] - vec3(0, aventadorData.dimensionHeight, 0)), PxForceMode::eFORCE);
				}
				if (aventadorId == 1) {
					float amount = controller2.LeftTrigger();
					PxRigidBodyExt::addLocalForceAtLocalPos(*actor,
						PxVec3(sin(wheelA) * (-aventadorData.force*amount), 0, cos(wheelA) * (-aventadorData.force*amount)),
						Util::g2p(wheelPos[i] - vec3(0, aventadorData.dimensionHeight, 0)), PxForceMode::eFORCE);
				}
			}
			PxVec3 wspeed = PxRigidBodyExt::getVelocityAtPos(*actor, Util::g2p(transform*vec4(wheelPos[i], 1)));
			wspeed.y = 0;
			vec3 forwardv = proj(Util::p2g(wspeed), wheeld); forwardv.y = 0;
			vec3 frictionv = Util::p2g(wspeed) - forwardv;
			vec3 frictiond = -normalize(frictionv); frictiond.y = 0;
			PxVec3 sideFriction = Util::g2p(frictiond / (1 + tireHeat[i]))*min(wspeed.magnitude() * aventadorData.wheelSideFriction, aventadorData.wheelSideMaxFriction),
				brakeFriction = Util::g2p(-normalize(forwardv)*brakeForce);

			PxRigidBodyExt::addForceAtPos(*actor, sideFriction + brakeFriction,
				Util::g2p(transform*vec4(wheelPos[i] - vec3(0, aventadorData.dimensionHeight, 0), 1)), PxForceMode::eFORCE);
			tireHeat[i] += length(frictionv) *aventadorData.tireHeatIncrease[i];
		}
	}
}

void Aventador::updateSteering() {
	actor->addTorque(-actor->getAngularVelocity() * 20);
	if (aventadorData.isAI) {
		AiManager::aiSteering(wheelAngle, aventadorData.isFront, actor->getGlobalPose());
	}
	else {
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
		if (aventadorId == 1) {
			float amount = -1 * controller2.LeftStick_X();
			if ((amount > 0.25) || (amount < -0.25)) {
				wheelAngle = +(amount*aventadorData.wheelTurnRate);
			}
			else {
				wheelAngle *= aventadorData.wheelReurnRate;
			}
		}
		if (aventadorId == 0) {
			float amount = -1 * controller1.LeftStick_X();
			if ((amount > 0.25) || (amount < -0.25)) {
				wheelAngle = +(amount*aventadorData.wheelTurnRate);
			}
			else {
				wheelAngle *= aventadorData.wheelReurnRate;
			}
		}

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
	if (aventadorId == 0) {
		if (controller1.GetButtonPressed(1)) {
			PxVec3 v = actor->getLinearVelocity();
			tireHeat[2] += aventadorData.manualTireHeatIncrease*v.magnitude() * 2;
			tireHeat[3] += aventadorData.manualTireHeatIncrease*v.magnitude() * 2;
		}
	}
	if (aventadorId == 1) {
		if (controller2.GetButtonPressed(1)) {
			PxVec3 v = actor->getLinearVelocity();
			tireHeat[2] += aventadorData.manualTireHeatIncrease*v.magnitude() * 2;
			tireHeat[3] += aventadorData.manualTireHeatIncrease*v.magnitude() * 2;
		}
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
	if (aventadorId == 0) {
		if (controller1.GetButtonPressed(1)) {
			brakeForce = min(brakeForce + aventadorData.brakeSpeed, aventadorData.maxBrakeForce);
		}
	}
	if (aventadorId == 1) {
		if (controller2.GetButtonPressed(1)) {
			brakeForce = min(brakeForce + aventadorData.brakeSpeed, aventadorData.maxBrakeForce);
		}
		else {
			brakeForce = 0;
		}
	}
	else {
		brakeForce = 0;
	}
}

void Aventador::updateFuel() {
	bool onPath = Game::path->pointInPath(actor->getGlobalPose().p.x, actor->getGlobalPose().p.z);
	if (!onPath) {
		aventadorData.fuel--;
		PxVec3 dir;
		if (aventadorData.fuel == 0) {
			actor->addForce(PxVec3(0, 30, 0), PxForceMode::eIMPULSE);
			dir = PxVec3(rand() % 250, 5, rand() % 250);
			PxRigidBodyExt::addLocalForceAtLocalPos(*actor,
				dir, PxVec3(-0.5, 0, 0), PxForceMode::eIMPULSE);
			PxRigidBodyExt::addLocalForceAtLocalPos(*actor,
				PxVec3(0, -5, 0), PxVec3(0.5, 0, 0), PxForceMode::eIMPULSE);
			positionTightness *= .1; targetTightness *= .08;
		}
		if (aventadorData.fuel <= 0 && aventadorData.fuel > -120) {

			actor->addTorque(dir, PxForceMode::eIMPULSE);
			//game over flag
			if (aventadorData.fuel < -100) {
				Game::setGameOverFlag(true);
			}
		}
	}
	else {
		if (aventadorData.fuel < aventadorData.tankSize) {
			aventadorData.fuel += 5;
			if (aventadorData.fuel > aventadorData.tankSize)
				aventadorData.fuel = aventadorData.tankSize;
		}
	}
}

bool Aventador::hasPowerUp() {
	return aventadorData.powerStatus;
}

void Aventador::setPowerUpStatus(int status) {
	aventadorData.powerStatus = status;
}

void Aventador::usePowerUp() {
	if (Keyboard::keyDown(aventadorId ? GLFW_KEY_RIGHT_SHIFT : GLFW_KEY_F)) {
		if (aventadorData.powerStatus == 1) {
			cout << aventadorId << " is using power 1" << endl;
			Game::getBack()->actor->addForce(PxVec3(100, 0, 100), PxForceMode::eIMPULSE);
			aventadorData.powerStatus = 0;
		}
		else if (aventadorData.powerStatus == 2) {
			cout << aventadorId << " is using power 2" << endl;
			Game::getBack()->tireHeat[0] = 1000;
			Game::getBack()->tireHeat[1] = 1000;
			Game::getBack()->tireHeat[2] = 1000;
			Game::getBack()->tireHeat[3] = 1000;
			aventadorData.powerStatus = 0;
		}
		else if (aventadorData.powerStatus == 3) {
			cout << aventadorId << " is using power 3" << endl;
			PxRigidBodyExt::addLocalForceAtLocalPos(*actor,
				PxVec3(0, 0, 300), PxVec3(0, 0, 0), PxForceMode::eIMPULSE);
			aventadorData.powerStatus = 0;
		}
		else if (aventadorData.powerStatus == 4) {
			cout << aventadorId << " is using power 4" << endl;
			aventadorData.fuel = aventadorData.fuel + 250;
			cout << aventadorData.fuel << endl;
			aventadorData.powerStatus = 0;
		}
		else if (aventadorData.powerStatus == 5) {
			cout << aventadorId << " is using power 5" << endl;
			Game::getFront()->actor->addForce(PxVec3(0, 0, -100), PxForceMode::eIMPULSE);
			aventadorData.powerStatus = 0;
		}
	}
	if (aventadorId == 0) {
		if (controller1.GetButtonPressed(0)) {
			if (aventadorData.powerStatus == 1) {
				cout << aventadorId << " is using power 1" << endl;
				Game::getBack()->actor->addForce(PxVec3(100, 0, 100), PxForceMode::eIMPULSE);
				aventadorData.powerStatus = 0;
			}
			else if (aventadorData.powerStatus == 2) {
				cout << aventadorId << " is using power 2" << endl;
				Game::getBack()->tireHeat[0] = 1000;
				Game::getBack()->tireHeat[1] = 1000;
				Game::getBack()->tireHeat[2] = 1000;
				Game::getBack()->tireHeat[3] = 1000;
				aventadorData.powerStatus = 0;
			}
			else if (aventadorData.powerStatus == 3) {
				cout << aventadorId << " is using power 3" << endl;
				PxRigidBodyExt::addLocalForceAtLocalPos(*actor,
					PxVec3(0, 0, 300), PxVec3(0, 0, 0), PxForceMode::eIMPULSE);
				aventadorData.powerStatus = 0;
			}
			else if (aventadorData.powerStatus == 4) {
				cout << aventadorId << " is using power 4" << endl;
				aventadorData.fuel = aventadorData.fuel + 250;
				cout << aventadorData.fuel << endl;
				aventadorData.powerStatus = 0;
			}
			else if (aventadorData.powerStatus == 5) {
				cout << aventadorId << " is using power 5" << endl;
				Game::getFront()->actor->addForce(PxVec3(0, 0, -100), PxForceMode::eIMPULSE);
				aventadorData.powerStatus = 0;
			}
		}
	}
	if (aventadorId == 1) {
		if (controller2.GetButtonPressed(0)) {
			if (aventadorData.powerStatus == 1) {
				cout << aventadorId << " is using power 1" << endl;
				Game::getBack()->actor->addForce(PxVec3(100, 0, 100), PxForceMode::eIMPULSE);
				aventadorData.powerStatus = 0;
			}
			else if (aventadorData.powerStatus == 2) {
				cout << aventadorId << " is using power 2" << endl;
				Game::getBack()->tireHeat[0] = 1000;
				Game::getBack()->tireHeat[1] = 1000;
				Game::getBack()->tireHeat[2] = 1000;
				Game::getBack()->tireHeat[3] = 1000;
				aventadorData.powerStatus = 0;
			}
			else if (aventadorData.powerStatus == 3) {
				cout << aventadorId << " is using power 3" << endl;
				PxRigidBodyExt::addLocalForceAtLocalPos(*actor,
					PxVec3(0, 0, 300), PxVec3(0, 0, 0), PxForceMode::eIMPULSE);
				aventadorData.powerStatus = 0;
			}
			else if (aventadorData.powerStatus == 4) {
				cout << aventadorId << " is using power 4" << endl;
				aventadorData.fuel = aventadorData.fuel + 250;
				cout << aventadorData.fuel << endl;
				aventadorData.powerStatus = 0;
			}
			else if (aventadorData.powerStatus == 5) {
				cout << aventadorId << " is using power 5" << endl;
				Game::getFront()->actor->addForce(PxVec3(0, 0, -100), PxForceMode::eIMPULSE);
				aventadorData.powerStatus = 0;
			}
		}
	}
	if (Keyboard::keyDown(aventadorId ? GLFW_KEY_END : GLFW_KEY_T)) {
		cout << "throwing away powerup" << endl;
		aventadorData.powerStatus = 0;
	}
	if (aventadorId == 0) {
		if (controller1.GetButtonPressed(3)) {
			cout << "throwing away powerup" << endl;
			aventadorData.powerStatus = 0;
		}
	}
	if (aventadorId == 1) {
		if (controller2.GetButtonPressed(3)) {
			cout << "throwing away powerup" << endl;
			aventadorData.powerStatus = 0;
		}
	}
}

void Aventador::updateCurrentPowerUp(int type) {
	if (createBubble) {
		currentPowerUp.push_back(std::unique_ptr<PowerUpBubble>(new PowerUpBubble));
		if (type == 1)
			currentPowerUp[0]->material = &Resources::beet;
		else if (type == 2)
			currentPowerUp[0]->material = &Resources::brown;
		else if (type == 3)
			currentPowerUp[0]->material = &Resources::pink;
		else if (type == 4)
			currentPowerUp[0]->material = &Resources::teal;
		else if (type == 5)
			currentPowerUp[0]->material = &Resources::olive;
		else
			currentPowerUp[0]->material = &Resources::salmon;
		createBubble = false;
	}
	if (!hasPowerUp()) {
		currentPowerUp.clear();
	}
}

void Aventador::changeRole() {
	aventadorData.isFront = !aventadorData.isFront;
	aventadorData.fuel = aventadorData.tankSize;
	//jeremy changed the force speed too but i'm not sure where he puts it
	if (aventadorData.isFront) {
		aventadorData.force = forceFront;
	}
	else {
		aventadorData.force = forceBack;
	}
}

bool Aventador::isFront() {
	if (!this)return 0;
	return aventadorData.isFront;
}

void AventadorWheel::update(glm::mat4 parentTransform) {
	rotation += rotateSpeed*rotateInverse;
	rotation = rotation - 2 * PI*((int)(rotation / (2 * PI)));
	tempTransform = translate(transform, vec3(0.0f, height, 0.0f));
	tempTransform = rotate(tempTransform, facingAngle, vec3(0.0f, 1.0f, 0.0f));
	tempTransform = rotate(tempTransform, rotation, vec3(1.0f, 0.0f, 0.0f));
}

void AventadorWheel::renderShadowMap(glm::mat4 parentTransform) {
	Light::renderShadowMapInstanced(&Resources::aventadorWheel, parentTransform*tempTransform);
	Light::renderShadowMapInstanced(&Resources::aventadorWheelGlow, parentTransform*tempTransform);
}

void AventadorWheel::render(glm::mat4 parentTransform) {
	Graphics::RenderInstanced(&Resources::aventadorWheel, &Resources::darkGreyMaterial, parentTransform*tempTransform);
	Graphics::RenderInstanced(&Resources::aventadorWheelGlow, &material, parentTransform*tempTransform);
}

void PowerUpBubble::update(glm::mat4 parentTransform) {
	tempTransform = translate(transform, above);
}

void PowerUpBubble::render(glm::mat4 parentTransform) {
	Graphics::RenderInstanced(&Resources::powerUpBubble, material, parentTransform*tempTransform);
}