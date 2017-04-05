#pragma once

#include "PxPhysicsAPI.h"
#include <glm\glm.hpp>

using namespace physx;

namespace AiManager 
{
	void aiInit(bool &setIsAi);
	void aiSteering(float &wheelAngle, bool isFront, PxTransform globalPos);
	void moveTo(PxTransform origin, PxTransform target, float &wheelAngle);
	float getDist(PxVec3 a, PxVec3 b);
	bool isNearPowerUp();
};