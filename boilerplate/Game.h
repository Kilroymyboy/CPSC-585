#pragma once

#include <iostream>
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
#include "PowerUpList.h"

#define PRINT_FPS 0
#define PRINT_ENTITIES 0
#define VS_AI 1

namespace Game {
	extern std::list<std::shared_ptr<Entity> > entities;
	extern std::shared_ptr<Aventador> aventador0;
	extern std::shared_ptr<Aventador> aventador1;
	extern std::shared_ptr<Path> path;
	extern std::shared_ptr<HUDobj> hud;
	extern std::vector<PowerUp*> aiPowerUps;
	extern bool isGameOver;

	void init();
	void update();
	void switchRole();

	//int startScreen();
	//void endScreen();

	void addPowerUp();
	void createWall(glm::vec3);
	void checkDistance();
	void checkForSwap();
	double getDist();
	Aventador * getFront();
	Aventador * getBack();
	void setGameOverFlag(bool flag);
	bool didSwitchOccur();

}

namespace Time {
	extern double time, deltaTime;
	extern int fps;
	void init();
	void update();
}

