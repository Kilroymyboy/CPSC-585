#include "PowerUpList.h"
#include "Game.h"
#include "Aventador.h"

using namespace std;

//FRONT status value will be 1
void Blind::use()
{
	//glm::vec3 pos = Util::p2g(Game::getBack()->actor->getGlobalPose().p);
	//Game::createWall(pos , 4);
	cout << "TRUMP WALL" << endl;
}

//FRONT Status value will be 2
void BlackIce::use()
{
	Game::getBack()->Aventador::setPowerDuration(5);
	Game::getBack()->Aventador::settingTireHeat(true);
	cout << "slipping " << endl;
}

//FRONT status value will be 3
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

//BACK status value will be 5
void AutoPilot::use()
{
	Game::getBack()->Aventador::setPowerDuration(5);
	Game::getBack()->Aventador::settingAutoPilot(true);
	cout << "autopilot" << endl;
}

//BACK status value will be 6
void BoostBack::use()
{
	PxRigidBodyExt::addLocalForceAtLocalPos(*Game::getBack()->actor,
		PxVec3(0, 0, 200), PxVec3(0, 0, 0), PxForceMode::eIMPULSE);
	cout << "boosting back" << endl;
}

