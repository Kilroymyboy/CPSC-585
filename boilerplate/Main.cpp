#include "Graphics.h"
#include "Resources.h"
#include "Game.h"

int main(int argc, char *argv[])
{
	if (Graphics::init() == -1)return -1;
	Light::init();
	Viewport::init();
	Resources::init();
	Game::init();
	while (!Graphics::shouldClose()) {
		Graphics::clearFrameBuffer();
		Game::update();
		Graphics::update();
	}
	Graphics::destroy();
}