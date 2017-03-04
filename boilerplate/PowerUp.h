#pragma once

#include "Entity.h"
#include "Resources.h"
#include "Graphics.h"
#include "FilterGroup.h"

class PowerUp :public Entity {
	physx::PxRigidDynamic *actor;
	int powerId;

public:
	PowerUp(int id);
	void update(glm::mat4 parentTransform)override;
	void render(glm::mat4 parentTransform)override;
	physx::PxRigidDynamic *const getActor();
	PxVec3 PowerUp::getRandLocation();
};