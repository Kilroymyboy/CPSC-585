#pragma once

#include "Graphics.h"

namespace Resources {
	extern Graphics::MyGeometry aventadorBody, aventadorBodyGlow, aventadorUnder;
	extern Graphics::MyShader standardShader;

	void init();

	void nullMaterial();

	void darkGreyMaterial();

	void pureBlackMaterial();

	void emmisiveMaterial();
}