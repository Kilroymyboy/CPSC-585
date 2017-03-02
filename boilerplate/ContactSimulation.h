#pragma once
#include "PxPhysicsAPI.h"

using namespace physx;

class contactModifcation : public PxSimulationEventCallback {

public:
	void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs);

	//needs to be overidden to set as our simulationEventCallback
	void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) {}
	void onWake(PxActor** actors, PxU32 count) {}
	void onSleep(PxActor** actors, PxU32 count) {}
	void onTrigger(PxTriggerPair* pairs, PxU32 count) {}
};

struct FilterGroup {
	enum Enum {
		eAventador = (1 << 0),
		ePowerUp = (1 << 1),
	};
};