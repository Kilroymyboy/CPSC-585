#include "Path.h"

using namespace std;
using namespace glm;
using namespace physx;

const float thickness = 4;

const std::vector<glm::vec3> displacements{
	glm::vec3(-thickness,0,thickness),
	glm::vec3(thickness,0,thickness),
	glm::vec3(thickness,0,-thickness),
	glm::vec3(-thickness,0,-thickness)
};

Path::Path(int geometrySize) {
	size = geometrySize;
	this->aventador = Game::getFront();
	cooldown = 0.1;
	nextGenTime = Time::time + cooldown;
	for (int i = 0; i < (size + 1) * 6; i++) {
		positions.push_back(vec3(0, 0.01, 0));
		normals.push_back(vec3(0, 1, 0));
		uvs.push_back(vec2(0, 0));
	}
	Graphics::initGeometry(&geometry);
	geometry.elementCount = positions.size();

	glBindBuffer(GL_ARRAY_BUFFER, geometry.vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*positions.size(), &positions[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, geometry.textureBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2)*uvs.size(), &uvs[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, geometry.normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*normals.size(), &normals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Path::generate() {
	for (int i = 0; i < positions.size() - 6; i ++) {
		positions[i] = positions[i + 6];
	}
}

void Path::update(mat4 parentTransform) {
	if (Time::time > nextGenTime) {
		generate();
	}

	aventador = Game::getFront();

	vec4 frw(aventador->wheelPos[0], 1);
	vec4 flw(aventador->wheelPos[1], 1);
	frw = aventador->transform*frw;
	frw.y = 0.01;
	flw = aventador->transform*flw;
	flw.y = 0.01;
	positions[positions.size() - 6] = positions[positions.size() - 12 + 2];
	positions[positions.size() - 5] = vec3(frw);
	positions[positions.size() - 4] = vec3(flw);
	positions[positions.size() - 3] = positions[positions.size() - 12 + 2];
	positions[positions.size() - 2] = positions[positions.size() - 12 + 1];
	positions[positions.size() - 1] = vec3(frw);

	glBindBuffer(GL_ARRAY_BUFFER, geometry.vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*positions.size(), &positions[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Path::render(mat4 parentTransform) {
	Graphics::Render(&geometry, &Resources::defaultMaterial, parentTransform*transform);
}