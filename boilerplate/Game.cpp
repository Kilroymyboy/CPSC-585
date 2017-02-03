#include "Game.h"
#include "Resources.h"
using namespace std;
using namespace glm;

Aventador::Aventador() {
	wheel1 = std::unique_ptr<AventadorWheel>(new AventadorWheel);
	wheel1.get()->transform = translate(mat4(1), vec3(.851f, .331f, 1.282f));
	wheel2 = std::unique_ptr<AventadorWheel>(new AventadorWheel);
	wheel2.get()->transform = scale(translate(mat4(1), vec3(.858f, .356f, -1.427f)), vec3(1.07f, 1.07f, 1.07f));

	wheel1.get()->rotateSpeed = .1f;
	wheel2.get()->rotateSpeed = .1f;

	wheel0 = std::unique_ptr<AventadorWheel>(new AventadorWheel);
	wheel0.get()->transform = rotate(translate(mat4(1), vec3(-.851f, .331f, 1.282f)), (float)PI, vec3(0, 1, 0));
	wheel3 = std::unique_ptr<AventadorWheel>(new AventadorWheel);
	wheel3.get()->transform = scale(rotate(translate(mat4(1), vec3(-.858f, .356f, -1.427f)), (float)PI, vec3(0, 1, 0)), vec3(1.07f, 1.07f, 1.07f));

	wheel0.get()->rotateSpeed = -.1f;
	wheel3.get()->rotateSpeed = -.1f;
}

void Aventador::update(glm::mat4 parentTransform) {
	//	transform = rotate(transform, 0.005f, vec3(0.0f, 0.0f, 1.0f));
	Graphics::RenderScene(&Resources::aventadorBody, &Resources::standardShader, &(Resources::darkGreyMaterial), parentTransform* transform);
	Graphics::RenderScene(&Resources::aventadorBodyGlow, &Resources::standardShader, &Resources::emmisiveBlueMaterial, parentTransform*transform);
	Graphics::RenderScene(&Resources::aventadorUnder, &Resources::standardShader, &Resources::pureBlackMaterial, parentTransform*transform);

	wheel1.get()->update(parentTransform*transform);
	wheel2.get()->update(parentTransform*transform);
	wheel0.get()->update(parentTransform*transform);
	wheel3.get()->update(parentTransform*transform);
}

void AventadorWheel::update(glm::mat4 parentTransform) {
	transform = rotate(transform, rotateSpeed, vec3(1.0f, 0.0f, 0.0f));
	Graphics::RenderScene(&Resources::aventadorWheel, &Resources::standardShader, &Resources::darkGreyMaterial, parentTransform*transform);
	Graphics::RenderScene(&Resources::aventadorWheelGlow, &Resources::standardShader, &Resources::emmisiveBlueMaterial, parentTransform*transform);
}

namespace Game {
	vector<unique_ptr<Entity>> entities;

	// we can customize this function as much as we want for now for debugging
	void init() {
		entities.push_back(unique_ptr<Aventador>(new Aventador));
	}

	void update() {
	//	cout << Time::fps << "\t" << Time::deltaTime << endl;
		for (int i = 0; i < entities.size(); i++) {
			entities[i].get()->update(mat4(1));
		}
	}
}

namespace Time {
	double time, prevTime, deltaTime, fpsTime;
	int fps, tfps;

	void init() {
		time = glfwGetTime();
		prevTime = 0;
		deltaTime = 0;
		fps = tfps = 0;
		fpsTime = 0;
	}

	void update() {
		time = glfwGetTime();
		deltaTime = time - prevTime;
		prevTime = time;
		if (time > fpsTime + 1) {
			fps = tfps;
			tfps = 0;
			fpsTime = time;
		}
		else {
			tfps++;
		}
	}
}