#pragma once

#include "Graphics.h"
#include "Game.h"

class Path :public Entity {
	Graphics::MyGeometry geometry;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;
	int size;
	double cooldown;
	double nextGenTime;
	std::shared_ptr<Aventador> aventador;

	// generate the geometry again
	void generate();
public:
	void update(glm::mat4 parentTransform)override;
	void render(glm::mat4 parentTransform)override;
	Path(int geometrySize, std::shared_ptr<Aventador> aventador);
};