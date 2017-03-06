#include "Resources.h"

// TODO, destroy these things on exit?

namespace Resources {
	Graphics::MyGeometry aventadorBody, aventadorBodyGlow, aventadorUnder, aventadorWheel, aventadorWheelGlow;
	Graphics::MyGeometry plane, cube, centeredCube;
	Graphics::MyShader standardShader;

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

	Graphics::Material defaultMaterial(glm::vec3(1));
	Graphics::Material darkGreyMaterial(glm::vec3(.15));
	Graphics::Material emmisiveMaterial(glm::vec3(1), glm::vec3(1.7, 4.2, 11.2));
	Graphics::Material pureBlackMaterial(glm::vec3(0));
}