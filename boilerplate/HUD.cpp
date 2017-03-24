#include "HUD.h"
#include "Game.h"
#include <glm/gtx/matrix_decompose.hpp>

HUD::HUD()
{

}


HUD::~HUD()
{
}

void HUD::update()
{
	/*
	Game::hud->transform = glm::translate(glm::mat4(1), glm::vec3(0, 3, 0));
	Game::hud->transform = glm::scale(Game::hud->transform, glm::vec3(.1f, .1f, .1f));

	glm::mat4 transformation = Game::hud->transform;
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(transformation, scale, rotation, translation, skew, perspective);*/


//	Game::hud->transform = glm::inverse(Viewport::transform[0]) * glm::translate(glm::scale(glm::rotate(glm::mat4(1),90.0f, glm::vec3(1,0,0)),glm::vec3(.05,.05,.05)), glm::vec3(-10,-50,0));
	//Game::hud->transform = glm::rotate(Game::hud->transform, 90.0f, glm::vec3(1, 0, 0));
	//Game::hud->transform = glm::scale(Game::hud->transform, glm::vec3(0.1, 0.1, 0.1));
	//Game::hud->transform = glm::translate(Game::hud->transform, Viewport::position[0] + glm::vec3(0, -50, 0));
	//Game::hud->transform *= glm::inverse(Viewport::transform[0]);// *Game::hud->transform;
}