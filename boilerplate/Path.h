#pragma once

#include "Graphics.h"
#include "Aventador.h"

class Path :public Entity {
	Graphics::MyGeometry geometry;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> rpositions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;

	float displaceThickness = 0.25;
	float offset=1;
	int size;
	double cooldown;
	double nextGenTime;
	Aventador* aventador;


	// generate the geometry again
	void generate();
	void updateOffset(float &offset);
public:
	void update(glm::mat4 parentTransform)override;
	void render(glm::mat4 parentTransform)override;
	Path(int geometrySize);
	bool pointInPath(float x, float y);
	void setCenterPoints();

	std::vector<PxVec3> centerPoints;
};