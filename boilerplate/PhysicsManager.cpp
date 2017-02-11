#include "PhysicsManager.h"
#include "Game.h"

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
	PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	sceneDesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	sceneDesc.flags |= PxSceneFlag::eENABLE_ACTIVETRANSFORMS;
	mScene = mPhysics->createScene(sceneDesc);

	mMaterial = mPhysics->createMaterial(0.f, 0.f, 0.f);



	// infinite collision plane
	PxRigidStatic* groundPlane = PxCreatePlane(*mPhysics, PxPlane(0, 1, 0, 0), *mMaterial);
	//PxShape *shape = groundPlane->createShape(PxPlaneGeometry(), *mMaterial);
	mScene->addActor(*groundPlane);


	PxReal density = 1.0f;
	PxTransform transform(PxVec3(0.0f, 2.0f, 0.0f), PxQuat::createIdentity());
	PxVec3 dimensions(0.1f, 0.1f, 0.1f);
	PxBoxGeometry geometry(dimensions);


	actor = PxCreateDynamic(*mPhysics, transform, geometry, *mMaterial, density);
	mScene->addActor(*actor);
	actor->setAngularDamping(0.75);
	actor->setLinearVelocity(PxVec3(0, 0, 2));



	//for (PxU32 i = 0; i<5; i++)
	//	createStack(PxTransform(PxVec3(0, 0, 5)), 10, 2.0f);
	//createDynamic(PxTransform(PxVec3(0, 40, 100)), PxSphereGeometry(10), PxVec3(0, -50, -100));

	//applying force



	printf("PhysX initialized\n");

	//PxPhysics *mPhysics = NULL;
	//PxFoundation *mFoundation = NULL;
}


PhysicsManager::~PhysicsManager()
{
	printf("shutting down PhysX\n");

	//oppsite order of delcoration
	mScene->release();
	mPhysics->release();
	mFoundation->release();
	
}

void PhysicsManager::update(float delta)
{
	if (delta == 0) return;

	//mScene->simulate(delta);
	//mScene->fetchResults(true);

	mScene->simulate(1.0f / 60.0f);
	mScene->fetchResults(true);
	PxVec3 test = actor->getGlobalPose().p;

	PxU32 numTransforms;
	const PxActiveTransform *transforms = mScene->getActiveTransforms(numTransforms);


	for (PxU32 i = 0; i < numTransforms; ++i) 
	{
		glm::mat4 rotMatrix = glm::translate(glm::mat4(1), glm::vec3(transforms->actor2World.p.x, transforms->actor2World.p.y, transforms->actor2World.p.z));
		PxReal a; PxVec3 b; transforms->actor2World.q.toRadiansAndUnitAxis(a, b); rotMatrix = glm::rotate(rotMatrix, (float)a, glm::vec3(b.x, b.y, b.z));
		Game::entities[0]->transform = rotMatrix;
	}

			// glm::mat4_cast(glm::quat(transforms->actor2World.q.x, transforms->actor2World.q.y, transforms->actor2World.q.z, transforms->actor2World.q.w));



	//printf("1  this loc: %f , %f , %f\n", test.x, test.y, test.z);
	//printf("2  this loc: %f , %f , %f\n", transforms->actor2World.p.x, transforms->actor2World.p.y, transforms->actor2World.p.z);

	
	//PxRigidBodyExt::addForceAtLocalPos(*actor, PxVec3(0, 0, 5), PxVec3(0));
}

PxRigidDynamic* PhysicsManager::createDynamic(const PxTransform& t, const PxGeometry& geometry, const PxVec3& velocity = PxVec3(0))
{
	PxRigidDynamic* dynamic = PxCreateDynamic(*mPhysics, t, geometry, *mMaterial, 10.0f);
	dynamic->setAngularDamping(0.5f);
	dynamic->setLinearVelocity(velocity);
	mScene->addActor(*dynamic);
	return dynamic;
}

void PhysicsManager::createStack(const PxTransform& t, PxU32 size, PxReal halfExtent)
{
	PxShape* shape = mPhysics->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *mMaterial);
	for (PxU32 i = 0; i<size; i++)
	{
		for (PxU32 j = 0; j<size - i; j++)
		{
			PxTransform localTm(PxVec3(PxReal(j * 2) - PxReal(size - i), PxReal(i * 2 + 1), 0) * halfExtent);
			PxRigidDynamic* body = mPhysics->createRigidDynamic(t.transform(localTm));
			body->attachShape(*shape);
			PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
			mScene->addActor(*body);
		}
	}
	shape->release();
}