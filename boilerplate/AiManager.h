#pragma once

#include "PxPhysicsAPI.h"

using namespace physx;

namespace AiManager 
{
	void aiInit(bool &setIsAi, bool &setIsFront);
	void aiSteering(float &wheelAngle, /*float turnRate,*/ bool isFront, PxTransform globalPos);
	void findFront(PxVec3 direction, float &wheelAngle, float turnRate);
	void moveTo(PxTransform origin, PxTransform target, float &wheelAngle, float turnRate);
};