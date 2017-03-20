#include "Resources.h"
#include "Geometry.h"

// TODO, destroy these things on exit?

namespace Resources {
	Graphics::MyGeometry aventadorBody, aventadorBodyGlow, aventadorUnder, aventadorWheel, aventadorWheelGlow;
	Graphics::MyGeometry plane, cube, centeredCube;
	Graphics::MyShader standardShader;
	Graphics::MyGeometry gridLines;
	Graphics::MyTexture gridTexture;

	void init() {
		Graphics::loadGeometry(&aventadorBody, "models/aventador_body.obj");
		Graphics::loadGeometry(&aventadorBodyGlow, "models/aventador_body_glow.obj");
		Graphics::loadGeometry(&aventadorUnder, "models/aventador_black.obj");

		Graphics::loadGeometry(&aventadorWheel, "models/aventador_wheel.obj");
		Graphics::loadGeometry(&aventadorWheelGlow, "models/aventador_wheel_glow.obj");
		Graphics::loadGeometry(&plane, "models/planea.obj");
		Graphics::loadGeometry(&cube, "models/cube.obj");
		Graphics::loadGeometry(&centeredCube, "models/cube_center.obj");

		Geometry::initPlaneGeometry(&gridLines, 1000, 1000, 500);
		Graphics::InitializeShaders(&standardShader, "vertex.glsl", "fragment.glsl");
		if (!Graphics::InitializeTexture(&gridTexture, "images/grid1.png", GL_TEXTURE_2D, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR))
			std::cout << "can not initialize texture images/grid.jpg" << std::endl;

		planeMaterial.texture = gridTexture.textureID;
	}

	Graphics::Material defaultMaterial(glm::vec3(1));
	Graphics::Material darkGreyMaterial(glm::vec3(.15));
	Graphics::Material emmisiveMaterial(glm::vec3(1), glm::vec3(1.7, 4.2, 11.2));
	Graphics::Material pureBlackMaterial(glm::vec3(0));
	Graphics::Material coralMaterial(glm::vec3(1), glm::vec3(1.5, 0.75, 0.45));
	Graphics::Material paleGreenMaterial(glm::vec3(1), glm::vec3(0.75, 1.2, 0.75));
	Graphics::Material planeMaterial(glm::vec3(1));

}