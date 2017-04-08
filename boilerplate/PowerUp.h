#pragma once

#include "Entity.h"
#include "Resources.h"
#include "Graphics.h"
#include "FilterGroup.h"

class PowerUp : public Entity {
	physx::PxRigidDynamic *actor;
	double countDown = 20.0;
	double deleteTime;

	PxVec3 direction;
	PxTransform t;

	int pseudoRand();

public:
	PowerUp();
	void update(glm::mat4 parentTransform)override;
	void render(glm::mat4 parentTransform)override;
	physx::PxRigidDynamic *const getActor();
	PxVec3 getRandLocation();
	virtual void use() {};

	int powerId;
};