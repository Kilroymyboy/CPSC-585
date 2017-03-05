#include "Resources.h"

using namespace glm;
// TODO, destroy these things on exit?

namespace Resources {
	Graphics::MyGeometry aventadorBody, aventadorBodyGlow, aventadorUnder, aventadorWheel, aventadorWheelGlow;
	Graphics::MyGeometry plane, cube, centeredCube;
	Graphics::MyShader standardShader;

	Graphics::StandardShaderMaterial defaultMaterialObject(vec3(1, 1, 1));
	Graphics::StandardShaderMaterial emmisiveMaterialObject(vec3(1, 1, 1), vec3(1.7, 4.2, 11));
	Graphics::StandardShaderMaterial blackMaterialObject(vec3(0, 0, 0));

	void init() {
		Graphics::loadGeometry(&aventadorBody, "models/aventador_body.obj");
		Graphics::loadGeometry(&aventadorBodyGlow, "models/aventador_body_glow.obj");
		Graphics::loadGeometry(&aventadorUnder, "models/aventador_black.obj");
		Graphics::InitializeShaders(&standardShader, "vertex.glsl", "fragment.glsl");

		Graphics::loadGeometry(&aventadorWheel, "models/aventador_wheel.obj");
		Graphics::loadGeometry(&aventadorWheelGlow, "models/aventador_wheel_glow.obj");
		Graphics::loadGeometry(&plane, "models/plane.obj");
		Graphics::loadGeometry(&cube, "models/cube.obj");
		Graphics::loadGeometry(&centeredCube, "models/cube_center.obj");
	}

	void nullMaterial() {

	}

	void defaultMaterial() {
		glUniform3f(COLOR_LOCATION, 1.0f, 1.0f, 1.0f);
		glUniform3f(EMISSION_COLOR_LOCATION, 0.0f, 0.0f, 0.0f);
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
		glUniform3f(COLOR_LOCATION, 0.0f, 0.0f, 0.0f);
		glUniform3f(EMISSION_COLOR_LOCATION, 1.75f, 4.25f, 11.25f);
	}

	void emmisiveMaterial() {
		glUniform3f(COLOR_LOCATION, 1.0f, 1.0f, 1.0f);
		glUniform3f(EMISSION_COLOR_LOCATION, 0.0f, 0.0f, 0.0f);
	}
}