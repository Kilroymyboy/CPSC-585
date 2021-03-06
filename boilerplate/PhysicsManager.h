#pragma once
#include "PxPhysicsAPI.h"
#include "Time.h"
#include "extensions\PxRigidBodyExt.h"
#include "FilterGroup.h"
#include <vector>
#include <string>

#include <iostream>

using namespace physx;

namespace PhysicsManager
{
	void init();
	void destroy();

	void update(float);
	physx::PxRigidDynamic* createDynamic(const PxTransform&, const PxVec3&, const PxVec3& = PxVec3(0));
	PxRigidDynamic* createDynamic2(const PxTransform& t, const PxVec3& dimensions);
	void attachSimulationShape(PxRigidDynamic *actor, const PxVec3& dimensions, PxReal distance);
	void attachTriggerShape(PxRigidDynamic *actor, const PxVec3& dimensions);
	void setContactFilter(PxRigidActor *actor, PxU32 filterGroup, PxU32 filterMask);
	void createStack(const PxTransform&, PxU32, PxReal);

	PxFilterFlags contactFilterShader(PxFilterObjectAttributes attributes0,
		PxFilterData filterData0, PxFilterObjectAttributes attributes1,
		PxFilterData filterData1, PxPairFlags& pairFlags,
		const void * constantBlock, PxU32 constantBlockSize);

	extern physx::PxPhysics *mPhysics;
	extern physx::PxScene *mScene;
};

class ContactBehaviourCallback : public PxSimulationEventCallback {

public:
	void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs);

	//needs to be overidden to set as our simulationEventCallback
	void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) {}
	void onWake(PxActor** actors, PxU32 count) {}
	void onSleep(PxActor** actors, PxU32 count) {}
	void onTrigger(PxTriggerPair* pairs, PxU32 count) {}

};

class ContactModifyCallback : public PxContactModifyCallback {
	void onContactModify(PxContactModifyPair* const pairs, PxU32 count);
};