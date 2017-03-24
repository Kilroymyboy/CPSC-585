#pragma once
#include "PowerUp.h"

class Boost: public PowerUp
{
public:
	void use();
};


// BACK
class AutoPilot : public PowerUp
{
public:
	void use();
};

class Cloak : public PowerUp
{
public:
	void use();
};
//BACK


//FRONT
class BlackIce : public PowerUp
{
public:
	void use();
};

class Blind : public PowerUp
{
public:
	void use();
};
//FRONT
