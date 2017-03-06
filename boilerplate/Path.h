#pragma once

#include "Graphics.h"
#include "Game.h"

class Path :public Entity {
	Graphics::MyGeometry geometry;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	int size;
	float thickness;
	double cooldown;
	double nextGenTime;
	void genBuffer();
public:
	void update(glm::mat4 parentTransform)override;
	void render(glm::mat4 parentTransform)override;
	Path(int geometrySize);
};