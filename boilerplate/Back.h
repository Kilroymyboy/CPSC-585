#pragma once

#include "Aventador.h"
#include "PhysicsManager.h"
#include "PowerUp.h"

//discusion
/* 
not sure which one we want to use a template, abstract or just make these separate rule classes for front and back and just have 
the aventadors switch rule class depending on position
*/
class Back
{
public:
	Back();
	~Back();

	void gameRules();
	bool gameLost();
	void ai(bool isAi);

private:
	float fuel = 1000;
	bool powerup;
	bool onPath;
	int powerupIndex;
};

