#include "Graphics.h"

int main(int argc, char *argv[])
{
	if (Graphics::init() == -1)return -1;
	Light::init();
	Viewport::init();
	while (!Graphics::shouldClose()) {
		Graphics::update();
	}
	Graphics::destroy();
}