#pragma once

#include "PxPhysicsAPI.h"

using namespace physx;

namespace AiManager 
{
	void aiInit(bool &setIsAi, bool &setIsFront);
	void aiSteering(float &wheelAngle, /*float turnRate,*/ bool isFront, PxTransform globalPos);
	void createActorDetectors(PxRigidDynamic *block1, PxRigidDynamic *block2, PxRigidDynamic *block3, PxTransform globalPos);

};