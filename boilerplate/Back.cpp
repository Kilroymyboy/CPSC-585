#include "Back.h"



Back::Back()
{
}


Back::~Back()
{
}

bool Back::gameLost() {
	if (fuel <= 0) {
		return true;
	}
}

void Back::gameRules() {
	/*if (onPath == true)
		return;
	else {
		fuel - 1;
	}*/
}

void Back::ai(bool isAi) {

}