#pragma once
#include "Graphics.h"
#include "PhysicsManager.h"
#include "Util.h"
#include "Resources.h"

class Entity {
public:
	bool alive = true;
	glm::mat4 transform;
	virtual void update(glm::mat4 parentTransform) {}
	virtual void renderShadowMap(glm::mat4 parentTransform) {}
	virtual void render(glm::mat4 parentTransform) {}
};

class Plane :public Entity {
public:
	void update(glm::mat4 parentTransform)override;
	void render(glm::mat4 parentTransform)override;
};

class HUDobj :public Entity {
public:
	void update(glm::mat4 parentTransform)override;
	void render(glm::mat4 parentTransform)override;
};

class Cube :public Entity {
public:
	void update(glm::mat4 parentTransform)override;
	void render(glm::mat4 parentTransform)override;
};

class CenteredCube :public Entity {
	physx::PxRigidDynamic *actor;
public:
	CenteredCube(glm::vec3);
	void update(glm::mat4 parentTransform)override;
	void render(glm::mat4 parentTransform)override;
};