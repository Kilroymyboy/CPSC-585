#include "PowerUpManager.h"
#include "Game.h"

using namespace std;

PowerUpManager::PowerUpManager() {
	double powerUpSpawnTime = Time::time;
}

void PowerUpManager::update(glm::mat4 parentTransform) {
	//adding more power ups into the scene
	if (Time::time > powerUpSpawnTime) {
		cout << "time: " << Time::time << "\npowerUpSpawnTime: " << powerUpSpawnTime << "\n";
		powerUpSpawnTime += spawnCoolDown;
		Game::entities.push_back(shared_ptr<Entity>(new PowerUp()));
	}
}