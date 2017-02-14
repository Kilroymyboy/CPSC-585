#pragma once

#include "Game.h"
#include "Resources.h"
#include "InputManager.h"

class AventadorWheel :public Entity {
public:
	float rotateSpeed;
	void update0(glm::mat4 parentTransform)override;
	void update(glm::mat4 parentTransform)override;
};

class Aventador : public Entity {
	glm::vec3 modelDisplacement;
	physx::PxRigidDynamic *actor;
public:
	std::unique_ptr<AventadorWheel> wheel1, wheel2, wheel3, wheel0;
	void update0(glm::mat4 parentTransform)override;
	void update(glm::mat4 parentTransform)override;
	Aventador();
};
