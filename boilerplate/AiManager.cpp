#include "AiManager.h"
#include "PhysicsManager.h"
#include "Game.h"

using namespace glm;

namespace AiManager {

	double dCoolDown = 2;
	double dChangeTime = Time::time += dCoolDown;
	PxVec3 carFront = PxVec3(0, 0, 1);	//The direction the car is facing

	void aiInit(bool &setIsAi) {
		setIsAi = true;
	}

	void aiSteering(float &wheelAngle, bool isFront, PxTransform globalPos) {

		if (isFront) {
		/*
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
			*/
		}

		if (!isFront) {
			PxTransform frontPos = Game::getFront()->actor->getGlobalPose();
			PxTransform thisPos = globalPos;
			PxVec3 direction = frontPos.p - globalPos.p;
			float distance = direction.magnitude();
			std::vector<PxVec3> pathPoints = Game::path->centerPoints;
			PxVec3 goToPoint = pathPoints[0];
			float thisDistToPoint;				//distance between the current poition and a point in the path
			float pointDistToFront;				//distance between the point in the path 
			float prevDistToPoint = 0;			//the furthest point within maxDist
			float ClosestPointToFront = 1000;	//closest point towards the front car
			float maxDist = 20.0f;				//max range
			if (distance < 5) {
				moveTo(thisPos, frontPos, wheelAngle);
			}
			else if (distance < 150) {
				//find a point that is closest to the front car, near the back car
				for (int i = 0; i < pathPoints.size() - 1; i++) {
					PxVec3 thisToPoint = pathPoints[i] - thisPos.p;
					thisDistToPoint = thisToPoint.magnitude();
					PxVec3 pointToFront = frontPos.p - pathPoints[i];
					pointDistToFront = pointToFront.magnitude();
					if (thisDistToPoint > prevDistToPoint && thisDistToPoint < maxDist && ClosestPointToFront > pointDistToFront) {
						prevDistToPoint = thisDistToPoint;
						ClosestPointToFront = pointDistToFront;
						goToPoint = pathPoints[i];
						std::cout << "goToPoint Updated " << i << "\n";
					}
				}
				PxTransform goToPointTransform(goToPoint, PxQuat::createIdentity());
				moveTo(thisPos, goToPointTransform, wheelAngle);
			}
			else {	//to straight to the front car
				moveTo(thisPos, frontPos, wheelAngle);
			}
		}

	}

	void adjustAngle(float &angle) {
		if (angle > 10) {
			angle = 6;
		}
		else if (angle > 5) {
			angle = 5;
		}
		else if (angle > 2) {
			angle = 1;
		}
	}

	void moveTo(PxTransform origin, PxTransform target, float &wheelAngle) {
		PxVec3 originDirection = origin.q.rotate(carFront);	//current direction of the origin
		//PxVec3 targetDirection = target.q.rotate(carFront);	//maybe later for when the front car is really close by
		PxVec3 dirToTarget = target.p - origin.p;	//direction to move to
		//get the angle
		PxVec3 crossProd = originDirection.cross(dirToTarget);
		float angle = crossProd.magnitude();
		//adjust angle values for smoother movements
		//adjustAngle(angle);
		//std::cout << "angle: " << angle << "\n";


		if (crossProd.y < 2.5f && crossProd.y > -2.5f) {
			wheelAngle = 0;
		}
		else if (crossProd.y > 2.5f) { //to the left
			wheelAngle = angle;
		}
		else if (crossProd.y < -2.5f) { //to the right
			wheelAngle = angle*-1;
		}

	}

	float cross2D(vec2 point1, vec2 point2) {
		return (point1.x*point2.y) - (point1.y*point2.x);
	}

}