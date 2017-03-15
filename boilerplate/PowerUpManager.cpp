#include "PowerUpManager.h"
#include "Game.h"
using namespace std;
using namespace glm;
using namespace physx;

PowerUpManager::PowerUpManager() {
	powerId = pseudoRand() % 2; //0 or 1;
	deleteTime = Time::time += countDown+20; //allows at most 6 power ups at a time

	PxTransform t(getRandLocation(), PxQuat::createIdentity());
	PxVec3 dimensions(0.5f, 0.5f, 0.5f);
	actor = PhysicsManager::createDynamic(t, dimensions);
	actor->userData = (void*)ContactModFlags::eIGNORE_CONTACT;
	//actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
	//PhysicsManager::attachTriggerShape(actor, dimensions);
	PhysicsManager::attachSimulationShape(actor, dimensions, 0);
	PhysicsManager::setContactFilter(actor, FilterGroup::ePowerUp, FilterGroup::eAventador);

	if (powerId == 0) {
		actor->setName("powerup0");
	}
	else {
		actor->setName("powerup1");
	}
}

void PowerUpManager::update(mat4 parentTransform) {

	mat4 m = glm::translate(mat4(1), vec3(actor->getGlobalPose().p.x, actor->getGlobalPose().p.y, actor->getGlobalPose().p.z));
	PxReal a; PxVec3 b;
	actor->getGlobalPose().q.toRadiansAndUnitAxis(a, b);
	m = rotate(m, (float)a, vec3(b.x, b.y, b.z));
	transform = m;

	Light::renderShadowMap(&Resources::centeredCube, transform);

	// power up time out
	if (Time::time > deleteTime) {
		deleteTime += countDown;
		alive = false;
	}
	
}

void PowerUpManager::render(mat4 parentTransform) {
	if (powerId == 0) {
		Graphics::Render(&Resources::centeredCube, &Resources::coralMaterial, transform);
	}
	else {
		Graphics::Render(&Resources::centeredCube, &Resources::paleGreenMaterial, transform);
	}
}

physx::PxRigidDynamic *const PowerUpManager::getActor() {
	return actor;
}

PxVec3 PowerUpManager::getRandLocation() {
	float x, y, z;
	x = (float)(pseudoRand() %20) - 10; //random number between -10 to 9
	y = 2.0f;
	z = (float)(pseudoRand() %20);
	
	return PxVec3(x, y, z);
}

int PowerUpManager::pseudoRand() {
	// our initial starting seed is 5323
	static unsigned int seed = 5323;
	seed = (8253729 * seed + 2396403);
	return seed;
}