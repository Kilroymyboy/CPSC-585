#pragma once
#include "PowerUp.h"
#include "Aventador.h"
#include <iostream>

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

// BACK at the moment is not implemented but just an idea
class Restore : public PowerUp
{
public:
	void use();
};
/*
//BACK
class Cloak : public PowerUp
{
public:
	void use();
};*/

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
