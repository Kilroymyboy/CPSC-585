#pragma once

#include <vector>
#include <list>
#include <memory>
#include "Graphics.h"
#include "PhysicsManager.h"

#define PRINT_FPS 0

class Entity {
public:
	bool alive = true;
	glm::mat4 transform;
	virtual void update0(glm::mat4 parentTransform) {}
	virtual void update(glm::mat4 parentTransform) {}
};

class AventadorWheel :public Entity {
public:
	float rotateSpeed;
	void update0(glm::mat4 parentTransform)override;
	void update(glm::mat4 parentTransform)override;
};

class Aventador : public Entity {
	glm::vec3 modelDisplacement;
	physx::PxRigidDynamic *actor;

public:
	std::unique_ptr<AventadorWheel> wheel1, wheel2, wheel3, wheel0;
	void update0(glm::mat4 parentTransform)override;
	void update(glm::mat4 parentTransform)override;
	Aventador();
};

class CenteredCube :public Entity {
	physx::PxRigidDynamic *actor;

public:
	CenteredCube(glm::vec3);
	void update0(glm::mat4 parentTransform)override;
	void update(glm::mat4 parentTransform)override;

	//physx::PxTransform getCenteredCubePos();
	//void setCenteredCubePos(physx::PxTransform newPos);
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

struct FilterGroup {
	enum Enum {
		eAventador = (1 << 0),
		eCenteredCube = (1 << 1),
		ePowerUp = (1 << 2),
	};
};

