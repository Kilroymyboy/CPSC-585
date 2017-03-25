#pragma once

#include "Entity.h"
#include "Resources.h"
#include "InputManager.h"
#include "Graphics.h"
#include "FilterGroup.h"
#include <vector>
#include <list>
#include <memory>

class VehicleData {
public:
	float springForce = 200;
	float damperForce = 12;
	float maxWheelAngle = PI / 5;
	float dimensionHeight = 0.45;
	float maxWheelDist = 0.46;
	float wheelTurnRate = 0.25;
	float wheelReurnRate = 0.85;

	float force = 45;
	float wheelSideFriction = 6.25;
	float wheelSideMaxFriction = 25;
	float topSpeedFriction = 0.8;

	float brakeSpeed = 0.8;
	float maxBrakeForce = 60;

	std::vector<float> tireHeatIncrease{ 0.006f,0.006f,0.02f,0.02f };
	float tireHeatDecrease = 0.45;
	float tireHeatFastDecrease = 0.1;
	float manualTireHeatIncrease = 0.15;

	bool powerStatus = false;
	bool isAI = false;
	bool hasLost = false;
	float fuel = 1000;

	bool isFront;

};

class AventadorWheel :public Entity {
public:
	float rotation = 0;
	float rotateSpeed;
	float rotateInverse;
	float facingAngle = 0;
	float height;
	glm::mat4 tempTransform;
	void update(glm::mat4 parentTransform)override;
	void renderShadowMap(glm::mat4 parentTransform)override;
	void render(glm::mat4 parentTransform)override;
};

class Aventador : public Entity {
	int aventadorId;

	physx::PxRigidDynamic *actor;

	glm::mat4 tempTransform;
	glm::mat3 inverseRotation;
	glm::vec3 modelDisplacement;

	std::vector<bool> wheelHit;
	std::vector<physx::PxRaycastBuffer> wheelHitInfo;

	void raycastWheels();
	void updateSuspension();
	void updateFriction();
	void updateSteering();
	void updateTopSpeed();
	void updateDrift();
	void updateBraking();

	void updateLightCamera();

	VehicleData aventadorData;

	float wheelAngle;
	float brakeForce;
	std::vector<float> tireHeat;

	double dCoolDown = 0.25;
	double dChangeTime;
	int randDirection;

public:
	// 0: front right, 1: front left, 2: rear left, 3: rear right
	std::vector<std::unique_ptr<AventadorWheel> > wheel;
	std::vector<glm::vec3> wheelPos;
	void update(glm::mat4 parentTransform)override;
	void renderShadowMap(glm::mat4 parentTransform)override;
	void render(glm::mat4 parentTransform)override;
	physx::PxRigidDynamic *const getActor();
	bool hasPowerUp();
	void setPowerUpStatus(bool status);
	void changeRole();
	Aventador(int);

	int Aventador::pseudoRand();
};