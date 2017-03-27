#include "AiManager.h"
#include "PhysicsManager.h"
#include "Game.h"

using namespace glm;

namespace AiManager {

	double dCoolDown = 2;
	double dChangeTime = Time::time += dCoolDown;
	float prevDist = 10000;
	double aChangeWait = 1;
	double aChangeTime = Time::time += aChangeWait;

	void aiInit(bool &setIsAi, bool &setIsFront) {
		setIsAi = true;
		setIsFront = true;
	}

	void aiSteering(float &wheelAngle, bool isFront, PxTransform globalPos) {

		if (isFront) {
			//float turnRate = rand() % 2;
			//if (Time::time > dChangeTime) {
			//	dChangeTime += dCoolDown;
			//	int randDirection = rand() % 3;
			//	if (randDirection == 0) {
			//		wheelAngle += turnRate;
			//	}
			//	else if (randDirection == 1) {
			//		wheelAngle -= turnRate;
			//	}
			//	else if (randDirection == 2) {
			//		wheelAngle *= turnRate;
			//	}
			//}
		}

		if (!isFront) {
			//left when x increases
			PxTransform frontPos = Game::getFront()->actor->getGlobalPose();
			PxVec3 direction = frontPos.p - globalPos.p;
			float distance = direction.magnitude();
			float turnRate = 0.01;
			if (distance < 150) { //if the car isn't that far behing the front, look for the path
				for (float i = 0.5; i < 1.5; i += 0.5) { //assign y axis of point
					float yCoordAhead = globalPos.p.z + i;
					for (float j = -1; j <= 1; j += 0.5) {
						if (Game::path->pointInPath(globalPos.p.x+j, yCoordAhead)) {	//check if the path is ahead of the car
							std::cout << "up ahead\n";
							wheelAngle *= turnRate;
							return;
						}
					}
					for (float j = 1; j < 4; j += 1.5) { //assign x axis of point
						float xCoordLeft = globalPos.p.x + j;
						float xCoordRight = globalPos.p.x - j;
						vec2 pointLeft = vec2(xCoordLeft, yCoordAhead);
						vec2 pointRight = vec2(xCoordRight, yCoordAhead);
						if (Game::path->pointInPath(pointRight.x, pointRight.y)) {	//check if the path is to the right of the car
							std::cout << "to the right\n";
							if (wheelAngle > -2)
								wheelAngle -= turnRate;
							return;
						}
						if (Game::path->pointInPath(pointLeft.x, pointLeft.y)) {	//check if the path is to the left of the car
							std::cout << "to the left\n";
							if (wheelAngle < 2)
								wheelAngle += turnRate;
							return;
						}
						else {	//the path is not nearby, turn towards the front car
							//move towards the front car
							std::cout << "where's the path?\n";
							//findFront(direction, wheelAngle, turnRate);
							moveTo(globalPos, frontPos, wheelAngle, turnRate);
						}
					}
				}
			}
			else {	//the front car is far away, move closer
				std::cout << "so far away\n";
				//findFront(direction, wheelAngle, turnRate);
				moveTo(globalPos, frontPos, wheelAngle, turnRate);
			}
			prevDist = distance;
		}

	}

	float cross2D(vec2 point1, vec2 point2) {
		return point1.x*point2.y - point1.y*point2.x;
	}

	void moveTo(PxTransform origin, PxTransform target, float &wheelAngle, float turnRate) {
		
		vec2 startPoint = vec2(origin.p.x, origin.p.z);
		vec2 endPoint = vec2(target.p.x, target.p.z);
		float cross = cross2D(startPoint, endPoint);

		if (cross == 0.0f) {
			wheelAngle *= 0;
		}
		else if (cross < 0.0f) {
			wheelAngle += turnRate;
		}
		else if (cross > 0.0f) {
			wheelAngle -= turnRate;
		}

	}

	void findFront(PxVec3 direction, float &wheelAngle, float turnRate) {

		//if (direction.x < 1) {
		//	std::cout << "to the right\n";
		//	if(wheelAngle > -1 )
		//		wheelAngle -= turnRate;
		//}
		//else if (direction.x > 1) {
		//	std::cout << "to the left\n";
		//	if(wheelAngle < 1)
		//		wheelAngle += turnRate;
		//}
		//else if (direction.x < 1 && direction.x > 1) {
		//	std::cout << "straight\n";
		//	wheelAngle *= turnRate;
		//}
	}

}