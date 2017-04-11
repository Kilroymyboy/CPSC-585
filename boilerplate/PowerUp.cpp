#include "PowerUp.h"
#include "Game.h"
#include <algorithm>

using namespace std;
using namespace glm;
using namespace physx;

PowerUp::PowerUp() {
	powerId = (int)pseudoRand() % 2; //0 or 1;
	deleteTime = Time::time += countDown;
	PxVec3 dimensions(0.5f, 0.5f, 0.5f);
	
	if (powerId == 0) {
		t = Game::aventador0->actor->getGlobalPose();
		if (VS_AI) {
			Game::aiPowerUps.push_back(this);
		}
	}
	else {
		t = Game::aventador1->actor->getGlobalPose();
	}

	PxTransform r(getRandLocation(), PxQuat::createIdentity());
	t.operator*=(r);
	actor = PhysicsManager::createDynamic2(t, dimensions);
	actor->userData = (void*)ContactModFlags::eIGNORE_CONTACT;
	PhysicsManager::attachSimulationShape(actor, dimensions, 0);
	PhysicsManager::setContactFilter(actor, FilterGroup::ePowerUp, FilterGroup::eAventador);

	if (powerId == 0) {
		actor->setName("powerup0");
	}
	else {
		actor->setName("powerup1");
	}
}

void PowerUp::update(mat4 parentTransform) {

	mat4 m = glm::translate(mat4(1), vec3(actor->getGlobalPose().p.x, actor->getGlobalPose().p.y, actor->getGlobalPose().p.z));
	PxReal a; PxVec3 b;
	actor->getGlobalPose().q.toRadiansAndUnitAxis(a, b);
	m = rotate(m, (float)a, vec3(b.x, b.y, b.z));
	transform = m;

	Light::renderShadowMap(&Resources::centeredCube, transform);

	if (changeType) {
		if (powerId == 0) {
			powerId = 1;
			actor->setName("powerup1");
		}
		else {
			powerId = 0;
			actor->setName("powerup0");
		}
		changeType = false;
	}

	if (contactErase) {
		actor->setName("erased");
	}

	if (Time::time > deleteTime) {
		if (powerId == 0 && VS_AI) {
			Game::aiPowerUps.erase(remove(Game::aiPowerUps.begin(), Game::aiPowerUps.end(), this), Game::aiPowerUps.end());
		}
		alive = false;
		actor->setName("erased");
	}
	
}

void PowerUp::render(mat4 parentTransform) {
	if (powerId == 0) {
		Graphics::RenderInstanced(&Resources::centeredCube, &Resources::coralMaterial, transform);
	}
	else {
		Graphics::RenderInstanced(&Resources::centeredCube, &Resources::paleGreenMaterial, transform);
	}
}

physx::PxRigidDynamic *const PowerUp::getActor() {
	return actor;
}

PxVec3 PowerUp::getRandLocation() {
	float x, y, z;
	x = (float)(pseudoRand() %30) - 15;
	y = 2.0f;
	z = (float)(pseudoRand() %50) + 100;	
	return PxVec3(x, y, z);
}

void PowerUp::use()
{
	printf("POWERUP ERROR\n");
}


int PowerUp::pseudoRand() {
	// our initial starting seed is 5323
	static unsigned int seed = 5323;
	seed = (8253729 * seed + 2396403);
	return seed;
}