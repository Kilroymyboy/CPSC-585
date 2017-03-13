#include "Geometry.h"

using namespace std;
using namespace glm;

namespace Geometry {

	void initGridLinesGeometry(Graphics::MyGeometry* geometry, int n, int m, float width, float height, float space) {
		vector<vec3> pos, nor;
		vector<vec2> texcoord;
		for (int i = -n / 2; i < (n + 1) / 2; i++) {
			pos.push_back(vec3(-height, 0, -width + i*space));
			pos.push_back(vec3(-height, 0, width + i*space));
			pos.push_back(vec3(height, 0, width + i*space));

			pos.push_back(vec3(height, 0, width + i*space));
			pos.push_back(vec3(height, 0, -width + i*space));
			pos.push_back(vec3(-height, 0, -width + i*space));

			for (int j = 0; j < 6; j++)	nor.push_back(vec3(0, 1, 0));
			for (int j = 0; j < 4; j++)	texcoord.push_back(vec2(0, 0));
		}
		for (int i = -n / 2; i < (n + 1) / 2; i++) {
			pos.push_back(vec3(-width + i*space, 0, -height));
			pos.push_back(vec3(width + i*space, 0, -height));
			pos.push_back(vec3(width + i*space, 0, height));

			pos.push_back(vec3(width + i*space, 0, height));
			pos.push_back(vec3(-width + i*space, 0, height));
			pos.push_back(vec3(-width + i*space, 0, -height));

			for (int j = 0; j < 6; j++)	nor.push_back(vec3(0, 1, 0));
			for (int j = 0; j < 4; j++)	texcoord.push_back(vec2(0, 0));
		}
		Graphics::initGeometry(geometry);
		Graphics::bufferGeometry(geometry, pos, texcoord, nor);
	}

	void initPlaneGeometry(Graphics::MyGeometry* geometry, float width, float height, float wrap) {
		vector<vec3> pos, nor;
		vector<vec2> texcoord;
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

		texcoord.push_back(vec2(0, wrap));
		texcoord.push_back(vec2(wrap, wrap));
		texcoord.push_back(vec2(0, 0));
		texcoord.push_back(vec2(0, 0));
		texcoord.push_back(vec2(wrap, wrap));
		texcoord.push_back(vec2(wrap, 0));

		Graphics::initGeometry(geometry);
		Graphics::bufferGeometry(geometry, pos, texcoord, nor);
	}

}