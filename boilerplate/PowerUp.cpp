#include "PowerUp.h"
using namespace std;
using namespace glm;
using namespace physx;

PowerUp::PowerUp(vec3 position) {
	PxTransform t(PxVec3(position.x, position.y, position.z), PxQuat::createIdentity());

	PxVec3 dimensions(0.5f, 0.5f, 0.5f);
	actor = PhysicsManager::createDynamic(t, dimensions);
	actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
	PhysicsManager::attachSimulationShape(actor, dimensions, 0); //may want to use a trigger shape for power ups
	PhysicsManager::setContactFilter(actor, FilterGroup::ePowerUp, FilterGroup::eAventador);
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
	Graphics::Render(&Resources::centeredCube, &Resources::emmisiveCoralMaterial, transform);
}

physx::PxRigidDynamic *const PowerUp::getActor() {
	return actor;
}