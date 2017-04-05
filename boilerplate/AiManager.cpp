#include "AiManager.h"
#include "PhysicsManager.h"
#include "Game.h"

using namespace glm;

namespace AiManager {

	double dCoolDown = 1;
	double dChangeTime = Time::time += dCoolDown;

	void aiInit(bool &setIsAi) {
		setIsAi = true;
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

			PxTransform frontPos = Game::getFront()->actor->getGlobalPose();
			PxTransform thisPos = globalPos;
			float distance = getDist(frontPos.p, globalPos.p);
			std::vector<PxVec3> pathPoints = Game::path->centerPoints;
			PxVec3 goToPoint = pathPoints[10];
			float thisDistToPoint;				//distance between the current poition and a point in the path
			float pointDistToFront;				//distance between the point in the path and tthe front car
			float prevDistToPoint = 0;			//the furthest point within maxDist
			float ClosestPointToFront = 1000;	//closest point towards the front car
			float maxDist = 20.0f;				//max range

			if (distance < 15) {	//if the front is nearby, go straight to it
				moveTo(thisPos, frontPos, wheelAngle);
			}
			else if (distance < 150) {	//find a point that is closest to the front car, near the back car
				for (int i = 0; i < pathPoints.size() - 1; i++) {
					thisDistToPoint = getDist(pathPoints[i], thisPos.p);
					pointDistToFront = getDist(frontPos.p, pathPoints[i]);
					if (thisDistToPoint > prevDistToPoint && thisDistToPoint < maxDist && ClosestPointToFront > pointDistToFront) {
						prevDistToPoint = thisDistToPoint;
						ClosestPointToFront = pointDistToFront;
						goToPoint = pathPoints[i];
						//std::cout << "goToPoint Updated " << i << "\n";
					}
				}
				PxTransform goToLoc(goToPoint, PxQuat::createIdentity());
				moveTo(thisPos, goToLoc, wheelAngle);
			}
			else {	//to straight to the front car
				moveTo(thisPos, frontPos, wheelAngle);
			}
		}

	}

	void moveTo(PxTransform origin, PxTransform target, float &wheelAngle) {

		PxVec3 originDirection = origin.q.rotate(PxVec3(0,0,1));	//current direction of the origin
		PxVec3 dirToTarget = target.p - origin.p;	//direction to move to
		//get the angle
		PxVec3 crossProd = originDirection.cross(dirToTarget);
		float angle = crossProd.magnitude();
		//std::cout << "angle: " << angle << "\n";

		if (crossProd.y < 1.5f && crossProd.y > -1.5f) {
			wheelAngle = 0;
		}
		else if (crossProd.y > 2.0f) { //to the left
			wheelAngle = angle;
		}
		else if (crossProd.y < -2.0f) { //to the right
			wheelAngle = angle*-1;
		}

	}

	float getDist(PxVec3 a, PxVec3 b) {
		PxVec3 direction = a - b;
		return direction.magnitude();
	}

	float cross2D(vec2 point1, vec2 point2) {
		return (point1.x*point2.y) - (point1.y*point2.x);
	}

}