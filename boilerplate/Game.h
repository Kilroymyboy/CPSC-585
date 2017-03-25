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
#include "Path.h"
#include "Aventador.h"
#include "PowerUp.h"


#define PRINT_FPS 0
#define PRINT_ENTITIES 0

namespace Game {
	extern std::list<std::shared_ptr<Entity> > entities;
	extern std::shared_ptr<Aventador> aventador0;
	extern std::shared_ptr<Aventador> aventador1;
	extern std::shared_ptr<Path> path;
	extern std::shared_ptr<HUDobj> hud;
	extern bool isGameOver;

	void init();
	void update();
	void switchRole();
	double getDist();
	Aventador * getFront();
	Aventador * getBack();
	void setGameOverFlag(bool flag);
	
}

namespace Time {
	extern double time, deltaTime;
	extern int fps;
	void init();
	void update();
}

