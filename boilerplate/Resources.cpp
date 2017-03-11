#include "Resources.h"
#include "Geometry.h"

// TODO, destroy these things on exit?

namespace Resources {
	Graphics::MyGeometry aventadorBody, aventadorBodyGlow, aventadorUnder, aventadorWheel, aventadorWheelGlow;
	Graphics::MyGeometry plane, cube, centeredCube, simplePlane;
	Graphics::MyShader standardShader;
	Graphics::MyGeometry gridLines;

	void init() {
		Graphics::loadGeometry(&aventadorBody, "models/aventador_body.obj");
		Graphics::loadGeometry(&aventadorBodyGlow, "models/aventador_body_glow.obj");
		Graphics::loadGeometry(&aventadorUnder, "models/aventador_black.obj");

		Graphics::loadGeometry(&aventadorWheel, "models/aventador_wheel.obj");
		Graphics::loadGeometry(&aventadorWheelGlow, "models/aventador_wheel_glow.obj");
		Graphics::loadGeometry(&plane, "models/planea.obj");
		Graphics::loadGeometry(&cube, "models/cube.obj");
		Graphics::loadGeometry(&centeredCube, "models/cube_center.obj");

		Geometry::initPlaneGeometry(&simplePlane, 7.5, 7.5);
		Geometry::initGridLinesGeometry(&gridLines, 100, 100, 0.25, 500, 7.5);
		Graphics::InitializeShaders(&standardShader, "vertex.glsl", "fragment.glsl");
	}

	Graphics::Material defaultMaterial(glm::vec3(1));
	Graphics::Material darkGreyMaterial(glm::vec3(.15));
	Graphics::Material emmisiveMaterial(glm::vec3(1), glm::vec3(1.7, 4.2, 11.2));
	Graphics::Material pureBlackMaterial(glm::vec3(0));
}