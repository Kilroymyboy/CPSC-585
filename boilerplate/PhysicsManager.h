#pragma once
#include "PxPhysicsAPI.h"
#include "Time.h"


using namespace physx;

class PhysicsManager
{
public:
	PhysicsManager();
	~PhysicsManager();

	void update();

private:
	PxFoundation *mFoundation;
	PxPhysics *mPhysics;
	PxScene *mScene;
};

