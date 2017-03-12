#include "Geometry.h"

using namespace std;
using namespace glm;

namespace Geometry {

	void initGridLinesGeometry(Graphics::MyGeometry* geometry, int n, int m, float width, float height, float space) {
		vector<vec3> pos, nor;
		for (int i = -n / 2; i < (n + 1) / 2; i++) {
			pos.push_back(vec3(-height, 0, -width + i*space));
			pos.push_back(vec3(-height, 0, width + i*space));
			pos.push_back(vec3(height, 0, width + i*space));

			pos.push_back(vec3(height, 0, width + i*space));
			pos.push_back(vec3(height, 0, -width + i*space));
			pos.push_back(vec3(-height, 0, -width + i*space));

			for (int j = 0; j < 6; j++)	nor.push_back(vec3(0, 1, 0));
		}
		for (int i = -n / 2; i < (n + 1) / 2; i++) {
			pos.push_back(vec3(-width + i*space, 0, -height));
			pos.push_back(vec3(width + i*space, 0, -height));
			pos.push_back(vec3(width + i*space, 0, height));

			pos.push_back(vec3(width + i*space, 0, height));
			pos.push_back(vec3(-width + i*space, 0, height));
			pos.push_back(vec3(-width + i*space, 0, -height));

			for (int j = 0; j < 6; j++)	nor.push_back(vec3(0, 1, 0));
		}
		Graphics::initGeometry(geometry);
		Graphics::bufferGeometry(geometry, pos, nor);
	}

	void initPlaneGeometry(Graphics::MyGeometry* geometry, float width, float height) {
		vector<vec3> pos, nor;
		pos.push_back(vec3(-height, .0f, width));
		pos.push_back(vec3(height, .0f, width));
		pos.push_back(vec3(-height, .0f, -width));

		pos.push_back(vec3(-height, .0f, -width));
		pos.push_back(vec3(height, .0f, width));
		pos.push_back(vec3(height, .0f, -width));

		nor.push_back(vec3(0.f, 1.f, 0.f));
		nor.push_back(vec3(0.f, 1.f, 0.f));
		nor.push_back(vec3(0.f, 1.f, 0.f));
		nor.push_back(vec3(0.f, 1.f, 0.f));
		nor.push_back(vec3(0.f, 1.f, 0.f));
		nor.push_back(vec3(0.f, 1.f, 0.f));

		Graphics::initGeometry(geometry);
		Graphics::bufferGeometry(geometry, pos, nor);
	}

}