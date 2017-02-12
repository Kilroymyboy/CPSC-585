#pragma once
#include "PxPhysicsAPI.h"
#include "Time.h"
#include "extensions\PxRigidBodyExt.h"
#include <vector>

using namespace physx;
namespace PhysicsManager
{
	void init();
	void destroy();

	void update(float);
	physx::PxRigidDynamic* createDynamic(const PxTransform&, const PxVec3&, const PxVec3& = PxVec3(0));
	void createStack(const PxTransform&, PxU32, PxReal);

	extern physx::PxPhysics *mPhysics;
	extern physx::PxScene *mScene;
};

