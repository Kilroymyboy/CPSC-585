#pragma once

#include "Game.h"
#include "Resources.h"
#include "InputManager.h"
#include "Graphics.h"
#include <vector>

class VehicleData {
public:
	float springForce = 200;
	float damperForce = 12;
	float maxWheelAngle = PI / 5;
	float dimensionHeight = 0.45;
	float maxWheelDist = 0.46;
};

class AventadorWheel :public Entity {
public:
	float rotation = 0;
	float rotateSpeed;
	float rotateInverse;
	float facingAngle = 0;
	float height;
	glm::mat4 tempTransform;
	void update0(glm::mat4 parentTransform)override;
	void update(glm::mat4 parentTransform)override;
};

class Aventador : public Entity {
	glm::mat4 tempTransform;
	glm::mat3 inverseRotation;
	glm::vec3 modelDisplacement;
	physx::PxRigidDynamic *actor;

	std::vector<bool> wheelHit;
	std::vector<physx::PxRaycastBuffer> wheelHitInfo;

	void raycastWheels();
	void updateSuspension();
	void updateFriction();
	void updateSteering();
	
	VehicleData aventadorData;
	float wheelAngle;
public:
	// 0: front right, 1: front left, 2: rear left, 3: rear right
	std::vector<std::unique_ptr<AventadorWheel> > wheel;
	std::vector<glm::vec3> wheelPos;
	void update0(glm::mat4 parentTransform)override;
	void update(glm::mat4 parentTransform)override;
	Aventador();
};
