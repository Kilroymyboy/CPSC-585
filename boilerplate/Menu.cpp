#include "Menu.h"

using namespace glm;

namespace Menu {
	void All::update(glm::mat4 parentTransform) {

	}
	void All::render(glm::mat4 parentTransform) {
		Graphics::Render(&Resources::allmenu, &Resources::teal, translate(mat4(1), vec3(0,2,0)));
	}

}

