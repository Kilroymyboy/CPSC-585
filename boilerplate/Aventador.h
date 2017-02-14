#pragma once

#include "Game.h"
#include "Resources.h"
#include "InputManager.h"
#include <vector>

class VehicleData {

};

class AventadorWheel :public Entity {
public:
	float rotation = 0;
	float rotateSpeed;
	float height;
	glm::mat4 tempTransform;
	void update0(glm::mat4 parentTransform)override;
	void update(glm::mat4 parentTransform)override;
};

class Aventador : public Entity {
	glm::mat4 tempTransform;
	glm::vec3 modelDisplacement;
	physx::PxRigidDynamic *actor;
	void updateSuspension();
	float springForce = 155;
	float damperForce = 5;
public:
	// 0: front right, 1: front left, 2: rear left, 3: rear right
	std::vector<std::unique_ptr<AventadorWheel> > wheel;
	std::vector<glm::vec3> wheelPos;
	void update0(glm::mat4 parentTransform)override;
	void update(glm::mat4 parentTransform)override;
	Aventador();
};
