#pragma once
#include "PxPhysicsAPI.h"
#include "Time.h"
#include "extensions\PxRigidBodyExt.h"
#include <vector>

namespace PhysicsManager
{
	void init();
	void destroy();

	void update(float);
	physx::PxRigidDynamic* createDynamic(const physx::PxTransform&, const physx::PxGeometry&, const physx::PxVec3&);
	void createStack(const physx::PxTransform&, physx::PxU32, physx::PxReal);
};

