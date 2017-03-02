#pragma once

#include "Entity.h"
#include "Resources.h"
#include "Graphics.h"
#include "ContactSimulation.h"

class PowerUp :public Entity {
	physx::PxRigidDynamic *actor;

public:
	PowerUp(glm::vec3);
	void update(glm::mat4 parentTransform)override;
	void render(glm::mat4 parentTransform)override;
	physx::PxRigidDynamic *const getActor();
};