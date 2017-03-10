#pragma once

#include <vector>
#include <list>
#include <memory>
#include "Graphics.h"
#include "PhysicsManager.h"
#include "Util.h"
#include "Resources.h"
#include "Entity.h"
#include "Aventador.h"
#include "PowerUp.h"


#define PRINT_FPS 0

namespace Game {
	extern std::list<std::shared_ptr<Entity> > entities;
	extern std::shared_ptr<Aventador> aventador0;
	extern std::shared_ptr<Aventador> aventador1;
	void init();
	void update();
	void switchRole();
}

namespace Time {
	extern double time, deltaTime;
	extern int fps;
	void init();
	void update();
}

