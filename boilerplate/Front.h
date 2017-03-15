#pragma once

#include "Aventador.h"
#include "PhysicsManager.h"
#include "PowerUp.h"

class Front
{
public:
	Front();
	~Front();

	void gameRules();
	void gameOver();
	void ai(bool isAi);

private:
	bool powerup;
	int powerupIndex;
};

