#pragma once

#include <vector>
#include <memory>
#include "Graphics.h"

class Entity {
public:
	virtual void update() {}
};

class Aventador : public Entity {
public:
	void update()override;
};

namespace Game {
	extern std::vector<std::unique_ptr<Entity> > entities;
	void init();
	void update();
}
