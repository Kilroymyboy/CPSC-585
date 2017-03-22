#pragma once

#include <vector>
#include <list>
#include <memory>
#include <math.h>
#include "Graphics.h"
#include "PhysicsManager.h"
#include "Util.h"
#include "Resources.h"
#include "Entity.h"
#include "Aventador.h"
#include "PowerUpManager.h"

#define PRINT_FPS 0
#define PRINT_ENTITIES 0

namespace Game {
	extern std::list<std::shared_ptr<Entity> > entities;
	extern std::shared_ptr<Aventador> aventador0;
	extern std::shared_ptr<Aventador> aventador1;
	extern std::shared_ptr<HUDobj> hud;

	void init();
	void update();
	void switchRole();
	double getDist();
	Aventador * getFront();
	Aventador * getBack();
	bool cnPointPolyTest(float x, float y, std::vector<glm::vec3> V, int n);
}

namespace Time {
	extern double time, deltaTime;
	extern int fps;
	void init();
	void update();
}

