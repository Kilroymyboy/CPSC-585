#include "PhysicsManager.h"
#include "Game.h"
#include "extensions\PxRigidBodyExt.h"


namespace PhysicsManager {

	enum Layer {
		LAYER0 = (1 << 0),
		LAYER1 = (1 << 1),
		LAYER2 = (1 << 2),
		LAYER3 = (1 << 3),
		LAYER4 = (1 << 4),
		LAYER5 = (1 << 5),
		LAYER6 = (1 << 6),
		LAYER7 = (1 << 7)
	};

	PxFoundation *mFoundation;
	PxPhysics *mPhysics;
	PxScene *mScene;
	PxMaterial *mMaterial;
	PxVisualDebuggerConnection* gConnection;
	ContactBehaviourCallback gSimulationEventCallback;
	ContactModifyCallback gContactModifyCallback;

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
		sceneDesc.simulationEventCallback = &gSimulationEventCallback;
		sceneDesc.contactModifyCallback = &gContactModifyCallback;
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

	void attachSimulationShape(PxRigidDynamic *actor, const PxVec3& dimensions, PxReal distance ) {
		//exclusion means that the shape is not shared among objects
		PxShape *shape = PxGetPhysics().createShape(PxBoxGeometry(dimensions), *mMaterial, true);
		shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
		actor->attachShape(*shape);
		shape->release();
	}

	void attachTriggerShape(PxRigidDynamic *actor, const PxVec3& dimensions) {
		//exclusion means that the shape is not shared among objects
		PxShape *shape = PxGetPhysics().createShape(PxBoxGeometry(dimensions), *mMaterial, true);
		shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
		shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
		actor->attachShape(*shape);
		shape->release();
	}

	//set up the filter flags
	void setContactFilter(PxRigidActor *actor, PxU32 filterGroup, PxU32 filterMask) {
		PxFilterData filterData;
		filterData.word0 = filterGroup; //filter ID of the actor
		filterData.word1 = filterMask; //filter ID that triggers a contact callback with the actor
		
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

	//Filter actors that contact each other
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

		//check if the filter group and mask match
		if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
		{
			pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eMODIFY_CONTACTS;
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

/******************************************************************************************************************************/

void ContactBehaviourCallback::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) {
	for (PxU32 i = 0; i < nbPairs; i++)
	{
		const PxContactPair& cp = pairs[i];

		if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			PxRigidActor* a0 = Game::aventador0->getActor();
			PxRigidActor* a1 = Game::aventador1->getActor();
			const char* name0 = "powerup0";
			const char* name1 = "powerup1";
			bool isAventador0 = pairHeader.actors[0] == a0 || pairHeader.actors[1] == a0;
			bool isAventador1 = pairHeader.actors[0] == a1 || pairHeader.actors[1] == a1;
			bool isPowerUp0 = pairHeader.actors[0]->getName() == name0 || pairHeader.actors[1]->getName() == name0;
			bool isPowerUp1 = pairHeader.actors[0]->getName() == name1 || pairHeader.actors[1]->getName() == name1;
			//bool isPowerUp = strcmp(pairHeader.actors[0]->getName(),"powerUp") || strcmp(pairHeader.actors[1]->getName(), "powerUp");


			if (isAventador0 && isAventador1) {
				std::cout << "Aventador made contact with another aventador\n";

				Game::switchRole();

				break;
			}
			else if (isPowerUp0 && isAventador0) {
				//remove the power up from the scene
				PxRigidActor* pickedUp = (pairHeader.actors[0]->getName() == name0) ? pairHeader.actors[0] : pairHeader.actors[1];
				auto power = find_if(Game::entities.begin(), Game::entities.end(), [&](std::shared_ptr<Entity>toFind) {
					PowerUpManager* power = static_cast<PowerUpManager*>(toFind.get());
					return power->getActor() == pickedUp; });
				if (power != Game::entities.end()) {
					Game::entities.erase(power);
				}
				std::cout << "aventador0 contacted a power up\n";
				//have aventador hold the power up. Holds one power up at a time
				Aventador* a = Game::aventador0.get();
				if (!a->hasPowerUp()) {
					a->setPowerUpStatus(true);
				}
				break;
			}
			else if (isPowerUp1 && isAventador1) {
				//remove the power up from the scene
				PxRigidActor* pickedUp = (pairHeader.actors[0]->getName() == name1) ? pairHeader.actors[0] : pairHeader.actors[1];
				auto power = find_if(Game::entities.begin(), Game::entities.end(), [&](std::shared_ptr<Entity>toFind) {
					PowerUpManager* power = static_cast<PowerUpManager*>(toFind.get());
					return power->getActor() == pickedUp; });
				if (power != Game::entities.end()) {
					Game::entities.erase(power);
				}
				std::cout << "aventador0 contacted a power up\n";
				//have aventador hold the power up. Holds one power up at a time
				Aventador* a = Game::aventador1.get();
				if (!a->hasPowerUp()) {
					a->setPowerUpStatus(true);
				}
				break;
			}
		}
	}
}

static void ignoreContacts(PxContactModifyPair& pair) {
	for (PxU32 i = 0; i < pair.contacts.size(); ++i) {
		pair.contacts.ignore(i);
	}
}

static void setTargetVelocity(PxContactModifyPair& pair, const PxVec3& targetVelocity) {
	for (PxU32 i = 0; i < pair.contacts.size(); ++i)
	{
		pair.contacts.setTargetVelocity(i, targetVelocity);
	}
}

void ContactModifyCallback::onContactModify(PxContactModifyPair* const pairs, PxU32 count) {

	for (PxU32 i = 0; i < count; i++) {
		PxU32 flags = static_cast<PxU32>(reinterpret_cast<size_t>(pairs[i].actor[0]->userData) | reinterpret_cast<size_t>(pairs[i].actor[0]->userData));

		if (flags & ContactModFlags::eIGNORE_CONTACT) {
			ignoreContacts(pairs[i]);
		}

		//increase the speed of the back car?
		//doesn't work yet
		const PxVec3 targetVelocity(0.f, 0.f, 100.f);
		bool isBackAventator = pairs->actor[0] == Game::aventador1->getActor() || pairs->actor[1] == Game::aventador1->getActor();
		if ((flags & ContactModFlags::eTARGET_VELOCITY)){
			setTargetVelocity(pairs[i],targetVelocity);
		}
	}
}
