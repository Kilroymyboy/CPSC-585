#include "Menu.h"
#include "GameExt.h"

using namespace glm;
using namespace std;

namespace Menu {
	All::All() {
		Viewport::position[0] = vec3(0, 2, 2);
		Viewport::target[0] = vec3(1, 2, 0);

		Viewport::position[1] = vec3(1, 2, 2);
		Viewport::target[1] = vec3(0, 2, 0);
	}

	void All::update(glm::mat4 parentTransform) {
		if (Keyboard::keyPressed(GLFW_KEY_ESCAPE)) {
			Game::startSinglePlayer();
		}

		Viewport::target[0] = vec3(1 + cos(Time::time / 2) / 4, 2, 0);
		Viewport::target[1] = vec3(0 + cos(Time::time / 2) / 4, 2, 0);
	}

	void All::render(glm::mat4 parentTransform) {
		Graphics::Render(&Resources::allmenu, &Resources::teal, translate(mat4(1), vec3(0, 2, 0)));
	}

}

