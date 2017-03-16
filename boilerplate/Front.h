#pragma once

#include "Aventador.h"
#include "PhysicsManager.h"
#include "PowerUp.h"

class Front
{
public:
	Front();
	~Front();

	virtual void gameRules() = 0;
	virtual void gameOver() = 0;
	virtual void ai(bool isAi) = 0;

private:
	bool powerup;
	int powerupIndex;
};

