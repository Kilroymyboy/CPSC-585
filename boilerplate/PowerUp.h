#pragma once

#include "Entity.h"
#include "Resources.h"
#include "Graphics.h"
#include "FilterGroup.h"

class PowerUp : public Entity {
	physx::PxRigidDynamic *actor;
	double countDown = 8.0;
	double deleteTime;

	int powerId;
	int pseudoRand();

public:
	PowerUp();
	void update(glm::mat4 parentTransform)override;
	void render(glm::mat4 parentTransform)override;
	physx::PxRigidDynamic *const getActor();
	PxVec3 getRandLocation();
};