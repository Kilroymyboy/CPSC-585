#pragma once

#include "Game.h"

namespace Menu {
	class All :public Entity {
		int selection;
	public:
		All();
		void update(glm::mat4 parentTransform)override;
		void render(glm::mat4 parentTransform)override;
	};
}

