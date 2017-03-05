#include "PowerUp.h"
using namespace std;
using namespace glm;
using namespace physx;

PowerUp::PowerUp(int id) {
	powerId = id;
	PxTransform t(getRandLocation(), PxQuat::createIdentity());
	PxVec3 dimensions(0.5f, 0.5f, 0.5f);
	actor = PhysicsManager::createDynamic(t, dimensions);
	actor->setName("powerup");

	/*
		note:	eIGNORE_CONACT causes the cubes to fall through the infinite ground plane
					-eDISABLE_GRAVITY is a work around for this
				This flag does not have the same effect for a aventators for some reason
				--I'll find out why from Modan later
	*/
	actor->userData = (void*)ContactModFlags::eIGNORE_CONTACT;
	actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
	//PhysicsManager::attachTriggerShape(actor, dimensions);
	PhysicsManager::attachSimulationShape(actor, dimensions, 0);
	if (powerId == 0) {
		PhysicsManager::setContactFilter(actor, FilterGroup::ePowerUp0, FilterGroup::eAventador0);
	}
	else {
		PhysicsManager::setContactFilter(actor, FilterGroup::ePowerUp1, FilterGroup::eAventador1);
	}
}

void PowerUp::update(mat4 parentTransform) {

	mat4 m = glm::translate(mat4(1), vec3(actor->getGlobalPose().p.x, actor->getGlobalPose().p.y, actor->getGlobalPose().p.z));
	PxReal a; PxVec3 b;
	actor->getGlobalPose().q.toRadiansAndUnitAxis(a, b);
	m = rotate(m, (float)a, vec3(b.x, b.y, b.z));
	transform = m;

	Light::renderShadowMap(&Resources::centeredCube, transform);

}

void PowerUp::render(mat4 parentTransform) {
	if (powerId == 0) {
		Graphics::Render(&Resources::centeredCube, &Resources::coralMaterial, transform);
	}
	else {
		Graphics::Render(&Resources::centeredCube, &Resources::paleGreenMaterial, transform);
	}
}

physx::PxRigidDynamic *const PowerUp::getActor() {
	return actor;
}

PxVec3 PowerUp::getRandLocation() {
	float x, y, z;
	x = (rand()%20)-10; //random number between -10 to 9
	y = 1.0f;
	z = (rand()%20)-10;
	
	return PxVec3(x, y, z);
}