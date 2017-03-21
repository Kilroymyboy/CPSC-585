#pragma once

#include "Entity.h"
#include <string>

class Skybox :public Entity {
	std::vector<glm::vec3> positions, normals;
	std::vector<glm::vec2> textcoords;

	std::vector<Graphics::MyGeometry> geometries;
	std::vector<Graphics::Material> materials;

	void initFace(int idx, char* s);
public:
	Skybox(float size);
	void update(glm::mat4 parentTransform)override;
	void renderShadowMap(glm::mat4 parentTransform)override;
	void render(glm::mat4 parentTransform)override;
};
