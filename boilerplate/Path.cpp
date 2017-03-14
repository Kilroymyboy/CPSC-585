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

void initPathGeometry(Graphics::MyGeometry* geometry, int size) {
	vector<vec3> normals;
	for (int i = 0; i < 6 * (size + 1); i++)
		normals.push_back(vec3(0, 1, 0));

	geometry->elementCount = 6 * (size + 1);

	glGenBuffers(1, &geometry->vertexBuffer);

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

Path::Path(int geometrySize, int wheel0, int wheel1) {
	size = geometrySize;
	initPathGeometry(&geometry, size);
	this->wheel0 = wheel0;
	this->wheel1 = wheel1;
	cooldown = 0.1;
	nextGenTime = Time::time + cooldown;
	for (int i = 0; i < (size + 1) * 2; i++) {
		positions.push_back(vec3(0, 0, 0));
	}
	genBuffer();
}

void Path::genBuffer() {

	vector<vec3> b;
	for (int i = 0; i < positions.size() - 3; i += 2) {
		b.push_back(positions[i]);
		b.push_back(positions[i + 1]);
		b.push_back(positions[i + 2]);

		b.push_back(positions[i + 1]);
		b.push_back(positions[i + 2]);
		b.push_back(positions[i + 3]);
	}
	glBindBuffer(GL_ARRAY_BUFFER, geometry.vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*b.size(), &b[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Path::update(mat4 parentTransform) {
	if (Time::time > nextGenTime) {
		nextGenTime += cooldown;

		vector<vec3> v;
		for (int i = 2; i < positions.size(); i++) {
			v.push_back(positions[i]);
		}
		mat4 m = Game::aventador0.get()->transform;
		vec3 pl = vec3(m*vec4(Game::aventador0.get()->wheelPos[wheel0] + displacements[wheel0], 1));
		vec3 pr = vec3(m*vec4(Game::aventador0.get()->wheelPos[wheel1] + displacements[wheel1], 1));
		pl.y = pr.y = 0;
		v.push_back(pl);
		v.push_back(pr);
		positions = v;
	}

	{
		positions.pop_back();
		positions.pop_back();
		mat4 m = Game::aventador0.get()->transform;
		vec3 pl = vec3(m*vec4(Game::aventador0.get()->wheelPos[wheel0] + displacements[wheel0], 1));
		vec3 pr = vec3(m*vec4(Game::aventador0.get()->wheelPos[wheel1] + displacements[wheel1], 1));
		pl.y = pr.y = 0;
		positions.push_back(pl);
		positions.push_back(pr);
	}

	genBuffer();

	Light::renderShadowMap(&geometry, parentTransform*transform);
}

void Path::render(mat4 parentTransform) {
	Graphics::Render(&geometry, &Resources::emmisiveMaterial, parentTransform*transform);
}