#pragma once

#include "Aventador.h"
#include "PhysicsManager.h"
#include "PowerUp.h"

//discusion
/*
Not sure if this front/back class is even needed and if it is, is it required to be an interface. For now it is an interface
*/
class Front
{
public:
	Front();
	~Front();

	virtual void gameWon() = 0;
	virtual void ai(bool isAi) = 0;

private:
	bool won = false;
	bool powerup;
	int powerupIndex;
};

