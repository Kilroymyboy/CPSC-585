#pragma once
#include "PowerUp.h"
#include "Aventador.h"
#include <iostream>

// BACK 
class AutoPilot : public PowerUp
{
public:
	void use()override;
};

// BACK 
class Restore : public PowerUp
{
public:
	void use()override;
};
//BACK 
class BoostBack : public PowerUp
{
public:
	void use()override;
};

//BACK 
class BoostFront : public PowerUp
{
public:
	void use()override;
};

//FRONT 
class BlackIce : public PowerUp
{
public:
	void use()override;
};

class Wind : public PowerUp
{
public:
	void use()override;
};