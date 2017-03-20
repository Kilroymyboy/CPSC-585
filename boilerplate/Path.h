#pragma once

#include "Graphics.h"
#include "Game.h"

class Path :public Entity {
	Graphics::MyGeometry geometry;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	Aventador *frontAventador;
	int size;
	double cooldown;
	double nextGenTime;

	int wheel0;
	int wheel1;

	void genBuffer();
public:
	void update(glm::mat4 parentTransform)override;
	void render(glm::mat4 parentTransform)override;
	Path(int geometrySize, int wheel0, int wheel1);
	std::vector<glm::vec3> getPathPoints();
};