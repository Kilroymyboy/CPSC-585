#include "AiManager.h"
#include "PhysicsManager.h"
#include "Game.h"

namespace AiManager {

	double dCoolDown = 2;
	double dChangeTime = Time::time += dCoolDown;

	void aiInit(bool &setIsAi, bool &setIsFront) {
		setIsAi = true;
		setIsFront = true;
	}

	void aiSteering(float &wheelAngle, bool isFront, PxTransform globalPos) {

		if (Time::time > dChangeTime) {
			dChangeTime += dCoolDown;
			float turnRate = rand() % 3;
			int randDirection = rand() % 3;
			if (randDirection == 0) {
				wheelAngle += turnRate;
			}
			else if (randDirection == 1) {
				wheelAngle -= turnRate;
			}
			else if (randDirection == 2) {
				wheelAngle *= turnRate;
			}
		}

		//if back



/*
		PxRigidDynamic *blockFront;
		PxRigidDynamic *blockFrontLeft;
		PxRigidDynamic *blockFrontRight;
		createActorDetectors(blockFrontLeft, blockFront, blockFrontRight, globalPos);
*/
	}

}