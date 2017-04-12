#pragma once

#include "Game.h"

namespace Menu {
	class All :public Entity {
	public:
		All();
		void update(glm::mat4 parentTransform)override;
		void render(glm::mat4 parentTransform)override;
	};
}

