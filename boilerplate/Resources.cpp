#include "Resources.h"

// TODO, destroy these things on exit?

namespace Resources {
	Graphics::MyGeometry aventadorBody, aventadorBodyGlow, aventadorUnder;
	Graphics::MyShader standardShader;
	void init() {
		Graphics::loadGeometry(&aventadorBody, "models/aventador_body.obj");
		Graphics::loadGeometry(&aventadorBodyGlow, "models/aventador_body_glow.obj");
		Graphics::loadGeometry(&aventadorUnder, "models/aventador_black.obj");
		Graphics::InitializeShaders(&standardShader, "vertex.glsl", "fragment.glsl");
	}

	void nullMaterial() {

	}

	void darkGreyMaterial() {
		glUniform3f(COLOR_LOCATION, 0.15f, 0.15f, 0.15f);
		glUniform3f(EMISSION_COLOR_LOCATION, 0.0f, 0.0f, 0.0f);
	}

	void pureBlackMaterial() {
		glUniform3f(COLOR_LOCATION, 0.0f, 0.0f, 0.0f);
		glUniform3f(EMISSION_COLOR_LOCATION, 0.0f, 0.0f, 0.0f);
	}

	void emmisiveBlueMaterial() {
		glUniform3f(COLOR_LOCATION, 1.0f, 1.0f, 1.0f);
		glUniform3f(EMISSION_COLOR_LOCATION, 1.0f, 1.0f, 5.0f);
	}
}