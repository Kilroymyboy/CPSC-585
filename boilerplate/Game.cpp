#include "Game.h"
#include "Resources.h"
using namespace std;

void Aventador::update() {
	Graphics::RenderScene(&Resources::aventadorBody, &Resources::standardShader, &(Resources::darkGreyMaterial));
	Graphics::RenderScene(&Resources::aventadorBodyGlow, &Resources::standardShader, &Resources::emmisiveMaterial);
	Graphics::RenderScene(&Resources::aventadorUnder, &Resources::standardShader, &Resources::pureBlackMaterial);
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