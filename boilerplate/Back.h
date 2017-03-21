#pragma once

#include "Aventador.h"
#include "PhysicsManager.h"
#include "PowerUp.h"

//discusion
/* 
Not sure if this front/back class is even needed and if it is, is it required to be an interface. For now it is an interface
*/
class Back
{
public:
	Back();
	~Back();

	virtual bool gameLost() = 0;
	virtual void ai(bool isAi) = 0;

private:
	float fuel = 1000;
	bool powerup = false;
	bool lost = false;
	bool onPath;
	int powerupIndex;
};

