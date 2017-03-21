#include "Skybox.h"

using namespace std;
using namespace glm;

void Skybox::initFace(int idx, char* s) {
	Graphics::initGeometry(&geometries[idx]);
	Graphics::bufferGeometry(&geometries[idx], positions, textcoords, normals);

	materials[idx] = Graphics::Material(vec3(0), vec3(0));
	Graphics::MyTexture texture;
	Graphics::InitializeTexture(&texture, s, GL_TEXTURE_2D, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR);
	materials[idx].texture = texture.textureID;
	materials[idx].useEmissive = vec3(1);
}

Skybox::Skybox(float size) {
	geometries.resize(6);
	materials.resize(6);

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
	initFace(0, "images/nebula_nz.png");

	// left
	for (int i = 0; i < 6; i++) {
		positions[i] = vec3(rotate(mat4(1), (float)(PI / 2), vec3(0, 1, 0))*vec4(positions[i], 1));
	}
	initFace(1, "images/nebula_nx.png");

	// front
	for (int i = 0; i < 6; i++) {
		positions[i] = vec3(rotate(mat4(1), (float)(PI / 2), vec3(0, 1, 0))*vec4(positions[i], 1));
	}
	initFace(2, "images/nebula_pz.png");

	// right
	for (int i = 0; i < 6; i++) {
		positions[i] = vec3(rotate(mat4(1), (float)(PI / 2), vec3(0, 1, 0))*vec4(positions[i], 1));
	}
	initFace(3, "images/nebula_px.png");
}

void Skybox::update(glm::mat4 parentTransform) {

}

void Skybox::renderShadowMap(glm::mat4 parentTransform) {

}

void Skybox::render(glm::mat4 parentTransform) {
	Graphics::Render(&geometries[0], &materials[0], parentTransform);
	Graphics::Render(&geometries[1], &materials[1], parentTransform);
	Graphics::Render(&geometries[2], &materials[2], parentTransform);
	Graphics::Render(&geometries[3], &materials[3], parentTransform);
}