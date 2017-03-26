#include "AiManager.h"
#include "PhysicsManager.h"
#include "Game.h"

using namespace glm;

namespace AiManager {

	double dCoolDown = 2;
	double dChangeTime = Time::time += dCoolDown;

	void aiInit(bool &setIsAi, bool &setIsFront) {
		setIsAi = true;
		//setIsFront = true;
		setIsFront = true;

	}

	void aiSteering(float &wheelAngle, bool isFront, PxTransform globalPos) {

		if (isFront) {
			float turnRate = rand() % 2;
			if (Time::time > dChangeTime) {
				dChangeTime += dCoolDown;
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
		}

		if (!isFront) {
			//left when x increases
			PxTransform frontPos = Game::getFront()->actor->getGlobalPose();
			PxVec3 direction = frontPos.p - globalPos.p;
			float magnitude = direction.magnitude;
			float turnRate = 5;
			for (float i = 0.5; i < 1.5; i+= 0.5) {
				float yCoordAhead = globalPos.p.z + i;
				if (Game::path->pointInPath(globalPos.p.x, yCoordAhead)) {
					wheelAngle *= 0;
					return;;
				}
				for (float j = 1; j < 2; j += 0.5) {
					float xCoordLeft = globalPos.p.x + j;
					float xCoordRight = globalPos.p.x - j;
					vec2 pointLeft = vec2(xCoordLeft, yCoordAhead);
					vec2 pointRight = vec2(xCoordRight, yCoordAhead);
					if (Game::path->pointInPath(pointRight.x, pointRight.y)) {
						wheelAngle -= turnRate;
						return;
					}
					else if (Game::path->pointInPath(pointLeft.x, pointLeft.y)) {
						wheelAngle += turnRate;
						return;
					}
					else {
						//move towards the front car
						if (direction.x < 0) {
							wheelAngle -= turnRate;
						}
						else if (direction.x > 0) {
							wheelAngle += turnRate;
						}
						else if (direction.x < 1 && direction.x > -1) {
							wheelAngle *= 0;
						}
					}
				}
			}
		}

	}

}