#include "PhysicsManager.h"

PhysicsManager::PhysicsManager()
{
	printf("initializing PhysX\n");

	// Physx Physics
	static PxDefaultErrorCallback gDefaultErrorCallback;
	static PxDefaultAllocator gDefaultAllocatorCallback;

	printf("creating Foundation\n");
	// create foundation object with default error and allocator callbacks.
	mFoundation = PxCreateFoundation(
		PX_PHYSICS_VERSION,
		gDefaultAllocatorCallback,
		gDefaultErrorCallback);

	printf("creating Physics\n");
	// create Physics object with the created foundation and with a 'default' scale tolerance.
	mPhysics = PxCreatePhysics(
		PX_PHYSICS_VERSION,
		*mFoundation,
		PxTolerancesScale());

	printf("creating scene\n");
	PxSceneDesc scenedesc(mPhysics->getTolerancesScale());
	scenedesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	scenedesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(1);
	scenedesc.flags |= PxSceneFlag::eENABLE_ACTIVETRANSFORMS;

	printf("PhysX initialized\n");


	PxPhysics *mPhysics = NULL;
	PxFoundation *mFoundation = NULL;
}


PhysicsManager::~PhysicsManager()
{
	printf("shutting down PhysX\n");

	//oppsite order of delcoration
	mScene->release();
	mPhysics->release();
	mFoundation->release();
}

void PhysicsManager::update()
{

}