#pragma once

#include "Graphics.h"

namespace Resources {
	extern Graphics::MyGeometry aventadorBody, aventadorBodyGlow, aventadorUnder, aventadorWheel, aventadorWheelGlow;
	extern Graphics::MyGeometry plane, cube, centeredCube, powerUpBubble;
	extern Graphics::MyShader standardShader;
	extern Graphics::MyGeometry gridLines;

	extern Graphics::MyTexture gridTexture;

	void init();

	extern Graphics::Material defaultMaterial;
	extern Graphics::Material darkGreyMaterial;
	extern Graphics::Material emmisiveMaterial;
	extern Graphics::Material pureBlackMaterial;

	extern Graphics::Material coralMaterial;
	extern Graphics::Material paleGreenMaterial;
	//http://cloford.com/resources/colours/500col.htm
	extern Graphics::Material beet;
	extern Graphics::Material brown;
	extern Graphics::Material pink;
	extern Graphics::Material teal;
	extern Graphics::Material olivedrab;
	extern Graphics::Material brightgray;
	extern Graphics::Material salmon;

	extern Graphics::Material planeMaterial;
}