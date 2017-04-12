#include "Menu.h"
#include "GameExt.h"

using namespace glm;
using namespace std;

namespace Menu {
	All::All() {
		selection = 0;

		Viewport::position[0] = vec3(0, 2, 2);
		Viewport::target[0] = vec3(1, 2, 0);

		Viewport::position[1] = vec3(1, 2, 2);
		Viewport::target[1] = vec3(0, 2, 0);
	}

	void All::update(glm::mat4 parentTransform) {
		if (Keyboard::keyPressed(GLFW_KEY_ENTER)) {
			if (selection == 0) {
				Game::startSinglePlayer();
			}
			if (selection == 1) {
				// start multiplayer here
				Game::startSinglePlayer();
			}
			if (selection == 2) {
				exit(0);
			}
		}

		if (Keyboard::keyPressed(GLFW_KEY_UP)) {
			selection += 2;
			selection %= 3;
		}
		if (Keyboard::keyPressed(GLFW_KEY_DOWN)) {
			selection += 1;
			selection %= 3;
		}

		Viewport::target[0] = vec3(1 + cos(Time::time / 2) / 4, 2, 0);
		Viewport::target[1] = vec3(0 + cos(Time::time / 2) / 4, 2, 0);
	}

	void All::render(glm::mat4 parentTransform) {
		Graphics::Render(&Resources::allmenu, &Resources::teal, translate(mat4(1), vec3(0, 2, 0)));
		if (selection == 0)
			Graphics::Render(&Resources::centeredCube, &Resources::teal, translate(scale(mat4(1), vec3(0.1f)), vec3(0, 20.25, 0)));
		if (selection == 1)Graphics::Render(&Resources::centeredCube, &Resources::teal, translate(scale(mat4(1), vec3(0.1f)), vec3(0, 17.5, 0)));
		if (selection == 2)Graphics::Render(&Resources::centeredCube, &Resources::teal, translate(scale(mat4(1), vec3(0.1f)), vec3(0, 13.5, 0)));
	}

}

