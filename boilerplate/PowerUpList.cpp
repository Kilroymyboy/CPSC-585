#include "PowerUpList.h"

using namespace std;

//It's status value will be 3
void Boost::use()
{
	cout << "boosting" << endl;
}


//BACK
//also no idea status value will be 1
void AutoPilot::use()
{
	cout << "autopilot" << endl;
}

//BACK Just an idea I had, a quick health replenish so you could run off the track and just when you are about to die use it to restore your fuel\
//status will be 2

void Restore::use()
{
	//aventador::aventadorData.fuel = aventadorData.tankSize;
	cout << "restore" << endl;
}


//BACK status will be 2
//possibly deprecated
/*id Cloak::use()
{
	cout << "cloaking " << endl;
}*/

//FRONT Status value will be 4
void BlackIce::use()
{
	//for (int i = 0; i < tireHeat.size(); i++)tireHeat[i] *= aventadorData.tireHeatFastDecrease;
	cout << "slipping " << endl;
}

//FRONT no idea status will be 5
void Blind::use()
{
	cout << "I am blind" << endl;
}