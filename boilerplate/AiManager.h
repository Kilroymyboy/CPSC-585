#pragma once

#include "PxPhysicsAPI.h"
#include <glm\glm.hpp>

using namespace physx;

namespace AiManager 
{
	void aiInit(bool &setIsAi, bool &setIsFront);
	void aiSteering(float &wheelAngle, bool isFront, PxTransform globalPos);
	void moveTo(PxTransform origin, PxTransform target, float &wheelAngle);
};