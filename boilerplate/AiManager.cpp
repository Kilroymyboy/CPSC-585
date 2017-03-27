#include "AiManager.h"
#include "PhysicsManager.h"
#include "Game.h"

using namespace glm;

namespace AiManager {

	double dCoolDown = 2;
	double dChangeTime = Time::time += dCoolDown;

	void aiInit(bool &setIsAi, bool &setIsFront) {
		setIsAi = true;
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
			float distance = direction.magnitude();
			float turnRate = 0.01;

			vec2 frontPos2D = vec2(frontPos.p.x, frontPos.p.z);
			vec2 currPos2D = vec2(globalPos.p.x, globalPos.p.z);

			if (distance < 150) { //if the car isn't that far behing the front, look for the path
				for (float i = 0.5; i < 1.5; i += 0.5) { //assign y axis of point
					float yCoordAhead = globalPos.p.z + i;
					for (float j = -1; j <= 1; j += 0.5) {
						if (Game::path->pointInPath(globalPos.p.x+j, yCoordAhead)) {	//check if the path is ahead of the car
							//std::cout << "up ahead\n";
							wheelAngle *= 0;
							return;
						}
					}
					for (float j = 1; j < 4; j += 1.5) { //assign x axis of point

						float xCoordLeft = globalPos.p.x + j;
						float xCoordRight = globalPos.p.x - j;
						vec2 pointLeft = vec2(xCoordLeft, yCoordAhead);
						vec2 pointRight = vec2(xCoordRight, yCoordAhead);
						if (Game::path->pointInPath(pointRight.x, pointRight.y)) {	//check if the path is to the right of the car
							//std::cout << "to the right\n";
							if (wheelAngle > -2)
								wheelAngle -= turnRate;
							return;
						}
						if (Game::path->pointInPath(pointLeft.x, pointLeft.y)) {	//check if the path is to the left of the car
							//std::cout << "to the left\n";
							if (wheelAngle < 2)
								wheelAngle += turnRate;
							return;
						}
						else {	//the path is not nearby, turn towards the front car
							//move towards the front car
							//std::cout << "where's the path?\n";
							//findFront(direction, wheelAngle, turnRate);
							moveTo(currPos2D, frontPos2D, wheelAngle, turnRate);
						}
					}
				}
			}
			else {	//the front car is far away, move closer
				//std::cout << "so far away\n";
				//findFront(direction, wheelAngle, turnRate);
				moveTo(currPos2D, frontPos2D, wheelAngle, turnRate);
			}
		}

	}

	float cross2D(vec2 point1, vec2 point2) {
		return (point1.x*point2.y) - (point1.y*point2.x);
	}

	vec2 normalize2D(vec2 v) {
		float magnitude = sqrt(v.x*v.x + v.y*v.y);
		return vec2(v.x/magnitude, v.y/magnitude);
	}

	void moveTo(vec2 origin, vec2 target, float &wheelAngle, float turnRate) {

		//not an improvement
		/*
		 float cross = cross2D(origin, target);

		if (cross == 0.0f) {
			std::cout << "move ahead\n";
			wheelAngle *= 0;
		}
		else if (cross < 0.0f) {
			std::cout << "move left\n";
			if (wheelAngle < 2)
				wheelAngle += turnRate;
		}
		else if (cross > 0.0f) {
			std::cout << "move right\n";
			if (wheelAngle > -2)
				wheelAngle -= turnRate;
		}
		*/

		vec2 direction = origin - target;
		vec2 move = normalize(direction);
		if (move.x == 0) {
			//std::cout << "move ahead\n";
			wheelAngle *= 0;
		}
		else if (move.x < 0) {
			//std::cout << "move left\n";
			if (wheelAngle < 3)
				wheelAngle += turnRate;
		}
		else if (move.x > 0) {
			//std::cout << "move right\n";
			if (wheelAngle > -3)
				wheelAngle -= turnRate;
		}

	}
}