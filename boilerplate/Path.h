#pragma once

#include "Graphics.h"
#include "Game.h"

class Path :public Entity {
	Graphics::MyGeometry geometry;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	int size;
	double cooldown;
	double nextGenTime;
public:
	void update0(glm::mat4 parentTransform)override;
	void update(glm::mat4 parentTransform)override;
	Path(int geometrySize);
};