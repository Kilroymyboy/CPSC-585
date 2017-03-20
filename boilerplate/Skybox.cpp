#include "Skybox.h"

using namespace std;
using namespace glm;

Skybox::Skybox(float size) {
	geometries.resize(6);
	materials.resize(6);

	vector<vec3> positions, normals;
	vector<vec2> textcoords;

	positions.resize(6);
	normals.assign(6, vec3(1));
	textcoords.resize(6);
	textcoords[0] = vec2(0, 0);
	textcoords[1] = vec2(1, 1);
	textcoords[2] = vec2(0, 1);
	textcoords[3] = vec2(0, 0);
	textcoords[4] = vec2(1, 0);
	textcoords[5] = vec2(1, 1);

	// back
	positions[0] = vec3(-size, -size, size);
	positions[1] = vec3(size, size, size);
	positions[2] = vec3(-size, size, size);
	positions[3] = vec3(-size, -size, size);
	positions[4] = vec3(size, -size, size);
	positions[5] = vec3(size, size, size);

	Graphics::initGeometry(&geometries[0]);
	Graphics::bufferGeometry(&geometries[0], positions, textcoords, normals);

	materials[0] = Graphics::Material(vec3(0), vec3(1));
}

void Skybox::update(glm::mat4 parentTransform) {

}

void Skybox::renderShadowMap(glm::mat4 parentTransform) {

}

void Skybox::render(glm::mat4 parentTransform) {

}