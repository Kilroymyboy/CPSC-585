#pragma once

#include <vector>
#include <memory>
#include "Graphics.h"

#define PRINT_FPS true

class Entity {
public:
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
public:
	std::unique_ptr<AventadorWheel> wheel1, wheel2, wheel3, wheel0;
	void update0(glm::mat4 parentTransform)override;
	void update(glm::mat4 parentTransform)override;
	Aventador();
};

namespace Game {
	extern std::vector<std::unique_ptr<Entity> > entities;
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