#pragma once

#include "Entity.h"
#include "Resources.h"
#include "Graphics.h"
#include "FilterGroup.h"

class PowerUpManager : public Entity {
	physx::PxRigidDynamic *actor;
	double countDown = 8.0;
	double deleteTime;
	PxVec3 direction;
	PxTransform t;

	int powerId;
	int pseudoRand();

public:
	PowerUpManager();
	void update(glm::mat4 parentTransform)override;
	void render(glm::mat4 parentTransform)override;
	physx::PxRigidDynamic *const getActor();
	PxVec3 PowerUpManager::getRandLocation();
};