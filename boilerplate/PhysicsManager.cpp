#include "PhysicsManager.h"
#include "Game.h"
#include "extensions\PxRigidBodyExt.h"

void contactModifcation::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) {
	for (PxU32 i = 0; i < nbPairs; i++)
	{
		const PxContactPair& cp = pairs[i];

		if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			//check if the collision is between two PxRigidDynamic actors. In our case, the Aventador and the CenteredCube
			//It appears that actor[0] is usually the Aventador
			if (pairHeader.actors[0]->getType() == PxActorType::Enum::eRIGID_DYNAMIC || pairHeader.actors[1]->getType() == PxActorType::Enum::eRIGID_DYNAMIC)
			{
				std::cout << "Rigid Dynamic touched Rigid Dynamic.\n";

				//force one of the actors to change positions. This eventually crashes the game lol
				PxRigidActor *actor1 = pairHeader.actors[0];
				PxRigidActor *actor2 = pairHeader.actors[1];
				PxTransform pose(PxVec3(0, 1, 0)); //this somehow affects the inital position of the centered cube
				actor1->setGlobalPose(pose);

				/*From SampleSubmarine
				PxActor* otherActor = (mSubmarineActor == pairHeader.actors[0]) ? pairHeader.actors[1] : pairHeader.actors[0];
				Seamine* mine = reinterpret_cast<Seamine*>(otherActor->userData);
				 insert only once
				if (std::find(mMinesToExplode.begin(), mMinesToExplode.end(), mine) == mMinesToExplode.end())
					mMinesToExplode.push_back(mine);
				*/

				break;
			}
			//TODO: check collision between powerups and Aventador
		}
	}
};

namespace PhysicsManager {


	PxFoundation *mFoundation;
	PxPhysics *mPhysics;
	PxScene *mScene;
	PxMaterial *mMaterial;
	PxVisualDebuggerConnection* gConnection;
	contactModifcation mySimulationEventCallback;


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
		sceneDesc.filterShader = contactFilterShader;
		sceneDesc.simulationEventCallback = &mySimulationEventCallback;
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

		dynamic->setAngularDamping(1.0f);
		dynamic->setLinearVelocity(velocity);
		mScene->addActor(*dynamic);
		return dynamic;
	}

	/*
	* attaching a simulation shape to the actor. This is used to detect collision.
	* release(): Decrements the reference count of a shape and releases it if the new reference count is zero
	*/
	void attachSimulationShape(PxRigidDynamic *actor, const PxVec3& dimensions, PxReal distance ) {
		//exclusion means that the shape is not shared among objects
		PxShape *shape = PxGetPhysics().createShape(PxBoxGeometry(dimensions), *mMaterial, true);
		shape->setContactOffset(distance); //conact triggers at a distance. 
		/*If the collision shapes are sized to be the exact same size 
		as the graphics shapes, a restOffset of zero is needed. 
		If the collision shapes are an epsilon bigger than the graphics shapes, 
		a restOffset of negative epsilon is correct.
		--question: confirm the size of the graphic shape*/
		shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
		actor->attachShape(*shape);
		shape->release();
	}

	//set up the filter flags
	void setContactFilter(PxRigidActor *actor, PxU32 filterGroup, PxU32 filterMask) {
		PxFilterData filterData;
		filterData.word0 = filterGroup; //filter ID of the actor
		filterData.word1 = filterMask; //filter ID that triggers a contact callback with the actor
		
		//actor should only have one shape, but need to do this to get the shape
		const PxU32 numShapes = actor->getNbShapes();
		PxShape** shapes = (PxShape**)malloc(sizeof(PxShape*)*numShapes);
		actor->getShapes(shapes, numShapes);
		for (PxU32 i = 0; i < numShapes; i++)
		{
			PxShape* shape = shapes[i];
			shape->setSimulationFilterData(filterData); //set up the filter data
		}
		free(shapes);
	}

	//the filter shader
	PxFilterFlags contactFilterShader(PxFilterObjectAttributes attributes0,
		PxFilterData filterData0, PxFilterObjectAttributes attributes1,
		PxFilterData filterData1, PxPairFlags& pairFlags,
		const void * constantBlock, PxU32 constantBlockSize)
	{
		// Check to see if either actor is a trigger
		if (PxFilterObjectIsTrigger(attributes0) ||
			PxFilterObjectIsTrigger(attributes1))
		{
			// Signal that a trigger has been activated and exit
			pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
			return PxFilterFlag::eDEFAULT;
		}

		// Generate a default contact report
		pairFlags |= PxPairFlag::eCONTACT_DEFAULT;

		// trigger the contact callback for pairs (A,B) where
		// the filtermask of A contains the ID of B and vice versa.
		if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
		{
			// Report the collision
			pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;
		}

		// Add the Continuous Collision Detection (CCD) flag, so that
		// CCD is enabled, and return the default filter flags
		pairFlags |= PxPairFlag::eCCD_LINEAR;
		return PxFilterFlag::eDEFAULT;

	}

	void updateSelf(const PxRigidDynamic *actor)
	{

	}
}


