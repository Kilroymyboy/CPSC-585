#include "Entity.h"
#include "Resources.h"
#include "InputManager.h"
#include "Aventador.h"
#include "Path.h"
#include "Resources.h"

using namespace std;
using namespace glm;
using namespace physx;


void Plane::update(glm::mat4 parentTransform) {

}

void Plane::render(glm::mat4 parentTransform) {
	Graphics::RenderInstanced(&Resources::gridLines, &Resources::planeMaterial, parentTransform);
}


void HUDobj::update(glm::mat4 parentTransform) {
	Light::renderShadowMap(&Resources::plane, parentTransform*transform);
}

void HUDobj::render(glm::mat4 parentTransform) {
	Graphics::Render(&Resources::plane, &Resources::emmisiveMaterial, parentTransform*transform);
}

//void Cube::update(glm::mat4 parentTransform) {
//	Light::renderShadowMap(&Resources::cube, parentTransform*transform);
//}

//void Cube::render(glm::mat4 parentTransform) {
//	Graphics::Render(&Resources::cube, &Resources::defaultMaterial, parentTransform*transform);
//}

CenteredCube::CenteredCube(vec3 position) {
	PxTransform t(PxVec3(position.x, position.y, position.z), PxQuat::createIdentity());

	PxVec3 dimensions(0.5f, 0.5f, 0.5f);
	actor = PhysicsManager::createDynamic(t, dimensions);
}

void CenteredCube::update(glm::mat4 parentTransform) {

	glm::mat4 m = glm::translate(glm::mat4(1), glm::vec3(actor->getGlobalPose().p.x, actor->getGlobalPose().p.y, actor->getGlobalPose().p.z));
	PxReal a; PxVec3 b;  actor->getGlobalPose().q.toRadiansAndUnitAxis(a, b); m = glm::rotate(m, (float)a, glm::vec3(b.x, b.y, b.z));
	transform = m;

	Light::renderShadowMap(&Resources::centeredCube, transform);
}

void CenteredCube::render(glm::mat4 parentTransform) {
	Graphics::Render(&Resources::centeredCube, &Resources::defaultMaterial, transform);
}