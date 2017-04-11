#include "Resources.h"
#include "Geometry.h"

// TODO, destroy these things on exit?

namespace Resources {
	Graphics::MyGeometry aventadorBody, aventadorBodyGlow, aventadorUnder, aventadorWheel, aventadorWheelGlow;
	Graphics::MyGeometry plane, cube, centeredCube, powerUpBubble;
	Graphics::MyShader standardShader;
	Graphics::MyGeometry gridLines;
	Graphics::MyTexture gridTexture;


	Graphics::MyGeometry allmenu;

	void init() {
		Graphics::loadGeometry(&aventadorBody, "models/aventador_body.obj");
		Graphics::loadGeometry(&aventadorBodyGlow, "models/aventador_body_glow.obj");
		Graphics::loadGeometry(&aventadorUnder, "models/aventador_black.obj");

		Graphics::loadGeometry(&aventadorWheel, "models/aventador_wheel.obj");
		Graphics::loadGeometry(&aventadorWheelGlow, "models/aventador_wheel_glow.obj");
		Graphics::loadGeometry(&plane, "models/planea.obj");
		Graphics::loadGeometry(&cube, "models/smallSphere.obj");
		Graphics::loadGeometry(&centeredCube, "models/cube_center.obj");
		Graphics::loadGeometry(&powerUpBubble, "models/smallSphere.obj");

		Graphics::loadGeometry(&allmenu, "models/texts/all.obj");

		Geometry::initPlaneGeometry(&gridLines, 1000, 1000, 500);
		Graphics::InitializeShaders(&standardShader, "vertex.glsl", "fragment.glsl");
		if (!Graphics::InitializeTexture(&gridTexture, "images/grid1.png", GL_TEXTURE_2D, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR))
			std::cout << "can not initialize texture images/grid.jpg" << std::endl;

		planeMaterial.texture = gridTexture.textureID;
		planeMaterial.useEmissive = glm::vec3(12.1/2.5,7.1/2.5,19.4/2.5);
	}

	Graphics::Material defaultMaterial(glm::vec3(1));
	Graphics::Material darkGreyMaterial(glm::vec3(.15));
	Graphics::Material emmisiveMaterial(glm::vec3(1), glm::vec3(1.8, 4.8, 12.6));
	Graphics::Material pureBlackMaterial(glm::vec3(0));
	Graphics::Material coralMaterial(glm::vec3(1), glm::vec3(1.5, 0.75, 0.45));
	Graphics::Material paleGreenMaterial(glm::vec3(1), glm::vec3(0.75, 1.2, 0.75));
	Graphics::Material planeMaterial(glm::vec3(1));

	Graphics::Material beet(glm::vec3(0.5), glm::vec3(1.22, 0.36, 1.22));
	Graphics::Material brown(glm::vec3(0.5), glm::vec3(1.39, 0.69, 0.19));
	Graphics::Material pink(glm::vec3(0.5), glm::vec3(1.075, 0.67, 0.725));
	Graphics::Material teal(glm::vec3(0.5), glm::vec3(0.36, 1.22, 1.22));
	Graphics::Material olive(glm::vec3(0.5), glm::vec3(1.05, 1.05, 0.25));
	Graphics::Material salmon(glm::vec3(0.5), glm::vec3(1.50, 0.20, 0.20));

	Graphics::Material pathMaterial(glm::vec3(1), glm::vec3(0.8, 1.5, 2.8));
}