#include "Game.h"
#include "Resources.h"
using namespace std;
using namespace glm;
void Aventador::update() {
	transform = translate(transform, vec3(0.0f, -.001f, 0.0f));
	Graphics::RenderScene(&Resources::aventadorBody, &Resources::standardShader, &(Resources::darkGreyMaterial), transform);
	Graphics::RenderScene(&Resources::aventadorBodyGlow, &Resources::standardShader, &Resources::emmisiveBlueMaterial, transform);
	Graphics::RenderScene(&Resources::aventadorUnder, &Resources::standardShader, &Resources::pureBlackMaterial, transform);
}

namespace Game {
	vector<unique_ptr<Entity>> entities;

	// we can customize this function as much as we want for now for debugging
	void init() {
		entities.push_back(unique_ptr<Aventador>(new Aventador));
	}

	void update() {
		for (int i = 0; i < entities.size(); i++) {
			entities[i].get()->update();
		}
	}
}