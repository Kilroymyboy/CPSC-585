#pragma once

#include "Entity.h"

class Skybox :public Entity {
	std::vector<Graphics::MyGeometry> geometries;
	std::vector<Graphics::Material> materials;
public:
	Skybox(float size);
	void update(glm::mat4 parentTransform)override;
	void renderShadowMap(glm::mat4 parentTransform)override;
	void render(glm::mat4 parentTransform)override;
};
