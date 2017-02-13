#pragma once

#include "Game.h"
#include "Resources.h"
#include "InputManager.h"
#include <vector>

class VehicleData {

};

class AventadorWheel :public Entity {
public:
	float rotateSpeed;
	void update0(glm::mat4 parentTransform)override;
	void update(glm::mat4 parentTransform)override;
};

class Aventador : public Entity {
	glm::vec3 modelDisplacement;
	physx::PxRigidDynamic *actor;
	void updateSuspension();
public:
	// 0: front right, 1: front left, 2: rear left, 3: rear right
	std::vector<std::unique_ptr<AventadorWheel> > wheel;
	std::vector<glm::vec3> wheelPos;
	void update0(glm::mat4 parentTransform)override;
	void update(glm::mat4 parentTransform)override;
	Aventador();
};
