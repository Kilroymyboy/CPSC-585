#pragma once

#include <vector>
#include <list>
#include <memory>
#include "Graphics.h"
#include "PhysicsManager.h"

#define PRINT_FPS 1

class Entity {
public:
	bool alive = true;
	glm::mat4 transform;
	virtual void update0(glm::mat4 parentTransform) {}
	virtual void update(glm::mat4 parentTransform) {}
};

namespace Game {
	extern std::list<std::unique_ptr<Entity> > entities;
	void init();
	void update();
}

namespace Time {
	extern double time, deltaTime;
	extern int fps;
	void init();
	void update();
}

class Plane :public Entity {
public:
	void update0(glm::mat4 parentTransform)override;
	void update(glm::mat4 parentTransform)override;
};

class Cube :public Entity {
public:
	void update0(glm::mat4 parentTransform)override;
	void update(glm::mat4 parentTransform)override;
};

class CenteredCube :public Entity {
	physx::PxRigidDynamic *actor;
public:
	CenteredCube(glm::vec3);
	void update0(glm::mat4 parentTransform)override;
	void update(glm::mat4 parentTransform)override;
};