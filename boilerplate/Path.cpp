#include "Path.h"

using namespace std;
using namespace glm;
using namespace physx;

void initPathGeometry(Graphics::MyGeometry* geometry, int size) {
	vector<vec3> positions, normals;
	for (int i = 0; i < 6 * size; i++)
		normals.push_back(vec3(0, 1, 0));
	for (int i = 0; i < 6 * (size - 1); i++)
		positions.push_back(vec3(i % 2 ? 5 : -5, 0, 0));

	positions.push_back(vec3(-5, 0, 0));
	positions.push_back(vec3(5, 0, 0));
	positions.push_back(vec3(-5, 0, 5));
	positions.push_back(vec3(-5, 0, 5));
	positions.push_back(vec3(5, 0, 5));
	positions.push_back(vec3(5, 0, 0));


	geometry->elementCount = 6 * size;

	glGenBuffers(1, &geometry->vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*positions.size(), &positions[0], GL_STATIC_DRAW);

	// create another one for storing our colours
	glGenBuffers(1, &geometry->normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*normals.size(), &normals[0], GL_STATIC_DRAW);

	// create a vertex array object encapsulating all our vertex attributes
	glGenVertexArrays(1, &geometry->vertexArray);
	glBindVertexArray(geometry->vertexArray);

	// associate the position array with the vertex array object
	glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBuffer);
	glVertexAttribPointer(VERTEX_POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(VERTEX_POSITION_LOCATION);

	// assocaite the colour array with the vertex array object
	glBindBuffer(GL_ARRAY_BUFFER, geometry->normalBuffer);
	glVertexAttribPointer(VERTEX_NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(VERTEX_NORMAL_LOCATION);

	// unbind our buffers, resetting to default state
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Path::Path(int geometrySize) {
	size = geometrySize;
	initPathGeometry(&geometry, size);
	cooldown = 0.5;
	nextGenTime = Time::time + cooldown;
	for (int i = 0; i < size; i++) {
		positions.push_back(vec3(i % 2 ? 5 : -5, 0, i * 5));
	}
}

void Path::update0(mat4 parentTransform) {
	if (Time::time > nextGenTime) {
		nextGenTime += cooldown;

		vector<vec3> v;
		for (int i = 2; i < size; i++) {
			v.push_back(v[i]);
		}
		mat4 m = Game::aventador.get()->transform;
	}

	Light::renderShadowMap(&geometry, parentTransform*transform);
}

void Path::update(mat4 parentTransform) {
	Graphics::RenderScene(&geometry, &Resources::standardShader, &Resources::defaultMaterial, parentTransform*transform);
}