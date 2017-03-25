#pragma once
#include "Entity.h"
#include "PowerUp.h"


class PowerUpManager : public Entity {

public:
	double spawnCoolDown = 5;
	double powerUpSpawnTime;
	PowerUpManager();
	void update(glm::mat4 parentTransform)override;
};