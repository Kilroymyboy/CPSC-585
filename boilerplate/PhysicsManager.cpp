#include "PhysicsManager.h"
#include "Game.h"
#include "extensions\PxRigidBodyExt.h"

namespace PhysicsManager {


	PxFoundation *mFoundation;
	PxPhysics *mPhysics;
	PxScene *mScene;
	PxMaterial *mMaterial;
	PxVisualDebuggerConnection* gConnection;

	//remove later
	PxRigidDynamic *player1;

	void init()
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

		mMaterial = mPhysics->createMaterial(0.0f, 0.3f, 0.f);

		// infinite collision plane
		PxRigidStatic* groundPlane = PxCreatePlane(*mPhysics, PxPlane(0, 1, 0, 0), *mMaterial);
		mScene->addActor(*groundPlane);

		PxReal density = 1.0f;
		PxTransform transform(PxVec3(0.0f, 2.0f, 0.0f), PxQuat::createIdentity());
		PxVec3 dimensions(0.1f, 0.1f, 0.1f);
		PxBoxGeometry geometry(dimensions);



		//for (PxU32 i = 0; i<5; i++)
		//	createStack(PxTransform(PxVec3(0, 0, 5)), 10, 2.0f);
		//createDynamic(PxTransform(PxVec3(0, 40, 100)), PxSphereGeometry(10), PxVec3(0, -50, -100));

		//applying force



		printf("PhysX initialized\n");

		//PxPhysics *mPhysics = NULL;
		//PxFoundation *mFoundation = NULL;
	}


	void destroy()
	{
		printf("shutting down PhysX\n");

		//oppsite order of delcoration
		mScene->release();
		mPhysics->release();
		mFoundation->release();

	}

	void update(float delta)
	{
		if (delta == 0) return;

		mScene->simulate(1.0f / 60.0f);
		mScene->fetchResults(true);

		PxU32 numTransforms;
		const PxActiveTransform *transforms = mScene->getActiveTransforms(numTransforms);


		//for (PxU32 i = 0; i < numTransforms; ++i)
		//{
		//	glm::mat4 rotMatrix = glm::translate(glm::mat4(1), glm::vec3(transforms->actor2World.p.x, transforms->actor2World.p.y, transforms->actor2World.p.z));
		//	PxReal a; PxVec3 b; transforms->actor2World.q.toRadiansAndUnitAxis(a, b); rotMatrix = glm::rotate(rotMatrix, (float)a, glm::vec3(b.x, b.y, b.z));
		//	Game::entities[0]->transform = rotMatrix;
		//}

		// glm::mat4_cast(glm::quat(transforms->actor2World.q.x, transforms->actor2World.q.y, transforms->actor2World.q.z, transforms->actor2World.q.w));
	}


	PxRigidDynamic* createDynamic(const PxTransform& t, const PxVec3& dimensions, const PxVec3& velocity)
	{
		PxBoxGeometry geometry(dimensions);
		PxRigidDynamic* dynamic = PxCreateDynamic(*mPhysics, t, geometry, *mMaterial, 1.0f);
		//actor = PxCreateDynamic(*PhysicsManager::mPhysics, t, geometry, *PhysicsManager::mPhysics->createMaterial(0.1f, 0.1f, 0.5f), PxReal(1.0f));

		dynamic->setAngularDamping(0.2f);
		dynamic->setLinearVelocity(velocity);
		mScene->addActor(*dynamic);
		return dynamic;
	}

	void updateSelf(const PxRigidDynamic *actor)
	{

	}
}