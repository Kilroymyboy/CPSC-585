#pragma once
#include "PowerUp.h"
#include "Aventador.h"
#include <iostream>

// BACK index 1
class AutoPilot : public PowerUp
{
public:
	void use()override;
};

// BACK index 2
class Restore : public PowerUp
{
public:
	void use()override;
};
//BACK index 3
class BoostBack : public PowerUp
{
public:
	void use()override;
};

//BACK index 4
class BoostFront : public PowerUp
{
public:
	void use()override;
};

//FRONT index 5
class BlackIce : public PowerUp
{
public:
	void use()override;
};

//FRONT index 6
class Blind : public PowerUp
{
public:
	void use()override;
};
