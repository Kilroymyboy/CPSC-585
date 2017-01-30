#include "Resources.h"

namespace Resources {
	Graphics::MyGeometry aventadorBody, aventadorBodyGlow, aventadorUnder;
	Graphics::MyShader standardShader;
	void init() {
		Graphics::loadGeometry(&aventadorBody, "models/aventador_body.obj");
		Graphics::loadGeometry(&aventadorBodyGlow, "models/aventador_body_glow.obj");
		Graphics::loadGeometry(&aventadorUnder, "models/aventador_under.obj");
		Graphics::InitializeShaders(&standardShader, "vertex.glsl", "fragment.glsl");
	}
}