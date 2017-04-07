#pragma once

#include "Entity.h"
#include "Resources.h"
#include "InputManager.h"
#include "Graphics.h"
#include "FilterGroup.h"
#include "PowerUp.h"
#include "PowerUpList.h"
#include "AiManager.h"
#include <vector>
#include <list>
#include <memory>

class VehicleData {
public:
	float springForce = 200;
	float damperForce = 12;
	float maxWheelAngle = PI / 5;
	float dimensionHeight = 0.45;
	float dimensionWidth = 1;
	float dimensionLength = 2.5;
	float maxWheelDist = 0.46;
	float wheelTurnRate = 0.5;
	float wheelReurnRate = 0.85;

	float force = 30;
	float wheelSideFriction = 6.25;
	float wheelSideMaxFriction = 25;
	float topSpeedFriction = 0.8;

	float brakeSpeed = 0.8;
	float maxBrakeForce = 60;


	std::vector<float> tireHeatIncrease{ 0.00f,0.00f,0.0f,0.0f }; 
	float tireHeatDecrease = 0.45;
	float tireHeatFastDecrease = 0.1;
	float manualTireHeatIncrease = 0.5;

	bool powerStatus = false; //if car has a powerup or not

	int fuel;
	int tankSize = 1000;

	bool playerAI = false; //is true if the aventador is the starting AI
	bool isAI = false; //is ai controlled
	bool hasLost = false;

	bool isFront;
	std::vector<PowerUp*> powerHeld; //vector holding a single powerup object using poweruplist use()
	bool slipActive; //if true the blackIce power has been used
	bool autoActive; //if true the autopilot power has been used
	double powerDuration; //duration each powerup is used for
};

class AventadorWheel :public Entity {
public:
	float rotation = 0;
	float rotateSpeed;
	float rotateInverse;
	float facingAngle = 0;
	float height;
	glm::mat4 tempTransform;
	void update(glm::mat4 parentTransform)override;
	void renderShadowMap(glm::mat4 parentTransform)override;
	void render(glm::mat4 parentTransform)override;
};

class Aventador : public Entity {
	int aventadorId;

	glm::mat4 tempTransform;
	glm::mat3 inverseRotation;
	glm::vec3 modelDisplacement;

	std::vector<bool> wheelHit;
	std::vector<physx::PxRaycastBuffer> wheelHitInfo;

	void raycastWheels();
	void updateSuspension();
	void updateFriction();
	void updateSteering();
	void updateTopSpeed();
	void updateDrift();
	void updateBraking();
	void updateFuel();

	void updateLightCamera();
	 
	VehicleData aventadorData;
	void usePowerUp();

	float wheelAngle;
	float brakeForce;
	std::vector<float> tireHeat;

public:
	physx::PxRigidDynamic *actor;
	// 0: front right, 1: front left, 2: rear left, 3: rear right
	std::vector<std::unique_ptr<AventadorWheel> > wheel;
	std::vector<glm::vec3> wheelPos;
	void update(glm::mat4 parentTransform)override;
	void renderShadowMap(glm::mat4 parentTransform)override;
	void render(glm::mat4 parentTransform)override;
	bool hasPowerUp();
	void setPowerUpStatus(int status);
	void changeRole();
	bool isFront();

	void setFuel(int increase); //used in the restore power to add fuel
	void settingTireHeat(bool val); //sets blackIce power to true
	void settingAutoPilot(bool val); //sets autoPilot power to true
	void setTireHeat(int heat); //set the tireheat if blackIce power is being used
	void setPowerDuration(double val); //sets the duration power length based off what power is being used

	void getPlayerAI(); //A separate get to get if the car was the starting AI

	Aventador(int);
};
