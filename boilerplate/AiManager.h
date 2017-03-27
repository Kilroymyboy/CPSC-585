#pragma once

#include "PxPhysicsAPI.h"
#include <glm\glm.hpp>

using namespace physx;

namespace AiManager 
{
	void aiInit(bool &setIsAi, bool &setIsFront);
	void aiSteering(float &wheelAngle, bool isFront, PxTransform globalPos);
	void moveTo(glm::vec2 origin, glm::vec2 target, float &wheelAngle, float turnRate);
};