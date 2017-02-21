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
	void attachSimulationShape(PxRigidDynamic *actor, const PxVec3& dimensions);
	void setupFiltering(PxRigidActor *actor, PxU32 filterGroup, PxU32 filterMask);
	void createStack(const PxTransform&, PxU32, PxReal);

	PxFilterFlags contactFilterShader(PxFilterObjectAttributes attributes0,
		PxFilterData filterData0, PxFilterObjectAttributes attributes1,
		PxFilterData filterData1, PxPairFlags& pairFlags,
		const void * constantBlock, PxU32 constantBlockSize);

	extern physx::PxPhysics *mPhysics;
	extern physx::PxScene *mScene;
};

