#pragma once
#include "PxPhysicsAPI.h"
#include "Time.h"
#include "extensions\PxRigidBodyExt.h"
#include <vector>

using namespace physx;

class PhysicsManager
{
public:
	PhysicsManager();
	~PhysicsManager();

	void update(float);
	PxRigidDynamic* createDynamic(const PxTransform&, const PxGeometry&, const PxVec3&);
	void createStack(const PxTransform&, PxU32, PxReal);

private:
	PxFoundation *mFoundation;
	PxPhysics *mPhysics;
	PxScene *mScene;
	PxMaterial *mMaterial;

	//remove later
	PxRigidDynamic *actor;
};

