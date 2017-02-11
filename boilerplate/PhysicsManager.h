#pragma once
#include "PxPhysicsAPI.h"
#include "Time.h"
#include "extensions\PxRigidBodyExt.h"
#include <vector>

class PhysicsManager
{
public:
	PhysicsManager();
	~PhysicsManager();

	void update(float);
	physx::PxRigidDynamic* createDynamic(const physx::PxTransform&, const physx::PxGeometry&, const physx::PxVec3&);
	void createStack(const physx::PxTransform&, physx::PxU32, physx::PxReal);

private:
	physx::PxFoundation *mFoundation;
	physx::PxPhysics *mPhysics;
	physx::PxScene *mScene;
	physx::PxMaterial *mMaterial;
	physx::PxVisualDebuggerConnection* gConnection;

	//remove later
	physx::PxRigidDynamic *actor;
};

