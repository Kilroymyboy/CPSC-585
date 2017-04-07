#include "PowerUpList.h"
#include "Game.h"
#include "Aventador.h"

using namespace std;

//FRONT no idea status will be 1
void Blind::use()
{
	Game::createWall();
	cout << "I am blind" << endl;
}

//FRONT Status value will be 2
void BlackIce::use()
{
	Game::getBack()->Aventador::setPowerDuration(5);
	Game::getBack()->Aventador::settingTireHeat(true);
	cout << "slipping " << endl;
}

//It's status value will be 3
void BoostFront::use()
{
	PxRigidBodyExt::addLocalForceAtLocalPos(*Game::getFront()->actor,
		PxVec3(0, 0, 200), PxVec3(0, 0, 0), PxForceMode::eIMPULSE);
	cout << "boosting front" << endl;
}

//BACK status will be 4
void Restore::use()
{
	Game::getBack()->Aventador::setFuel(250);
	cout << "fuel restored " << endl;
}

//BACK
//status value will be 5
void AutoPilot::use()
{
	/*Game::getBack()->Aventador::setPowerDuration(5);
	Game::getBack()->Aventador::settingAutoPilot(true);*/
	cout << "autopilot" << endl;
}

//Back 6 boost back player
void BoostBack::use()
{
	PxRigidBodyExt::addLocalForceAtLocalPos(*Game::getBack()->actor,
		PxVec3(0, 0, 200), PxVec3(0, 0, 0), PxForceMode::eIMPULSE);
	cout << "boosting back" << endl;
}

