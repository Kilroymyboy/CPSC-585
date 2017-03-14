#pragma once

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <iterator>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Specify that we want the OpenGL core profile before including GLFW headers
#ifndef LAB_LINUX
#include <glad/glad.h>
#else
#define GLFW_INCLUDE_GLCOREARB
#define GL_GLEXT_PROTOTYPES
#endif
#include <GLFW/glfw3.h>

#define PI 3.14159265359

#define VERTEX_POSITION_LOCATION 0
#define VERTEX_NORMAL_LOCATION 1
#define VERTEX_TEXCOORD_LOCATION 2
#define TRANSFORM_LOCATION 3
#define COLOR_LOCATION 7
#define EMISSION_COLOR_LOCATION 8
#define PROJECTION_LOCATION 9
#define VIEW_LOCATION 10
#define LIGHT_LOCATION 11
#define AMBIENT_LOCATION 12
#define SHADOW_MVP_LOCATION 13
#define SOFT_SHADOW_LOCATION 14

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define MSAA 2
#define VSYNC 1
#define SHADOWMAP_SIZE 1600

#define EFFECTS 1
#define HDR_ENABLED 1

#define PRINT_DRAW_CALLS 0

#define CHECK_GL_ERRORS 0


namespace Graphics {
	extern bool SPLIT_SCREEN;
	// 0 horizontal/side by side, 1 vertical/stacked
	extern int SPLIT_SCREEN_ORIENTATION;

	extern bool SHADOW;
	extern int SOFT_SHADOW;

	int init();
	int shouldClose();
	void update();
	void destroy();

	struct MyShader
	{
		// OpenGL names for vertex and fragment shaders, shader program
		GLuint  vertex;
		GLuint  fragment;
		GLuint  program;

		// initialize shader and program names to zero (OpenGL reserved value)
		MyShader() : vertex(0), fragment(0), program(0)
		{}
	};

	struct MyTexture
	{
		GLuint textureID;
		GLuint target;
		int width;
		int height;

		// initialize object names to zero (OpenGL reserved value)
		MyTexture() : textureID(0), target(0), width(0), height(0)
		{}
	};

	struct Material {
		glm::vec3 color;
		glm::vec3 emmisiveColor;
		GLuint texture;

		Material() :color(glm::vec3(0)), emmisiveColor(glm::vec3(0)) {}
		Material(glm::vec3 c) :color(c), emmisiveColor(glm::vec3(0)) {}
		Material(glm::vec3 c, glm::vec3 e) :color(c), emmisiveColor(e) {}

		Material(GLuint t) :color(glm::vec3(0)), emmisiveColor(glm::vec3(0)), texture(t) {}
		Material(glm::vec3 c, GLuint t) :color(c), emmisiveColor(glm::vec3(0)), texture(t) {}
		Material(glm::vec3 c, glm::vec3 e, GLuint t) :color(c), emmisiveColor(e), texture(t) {}
	};

	struct MyGeometry
	{
		// OpenGL names for array buffer objects, vertex array object
		GLuint  vertexBuffer;
		GLuint  textureBuffer;
		GLuint  normalBuffer;
		GLuint  vertexArray;
		GLsizei elementCount;
		GLuint  transformBuffer;
		GLuint  colorBuffer;
		GLuint  emissionColorBuffer;

		std::vector<glm::mat4> transforms;
		std::vector<Material*> materials;

		// initialize object names to zero (OpenGL reserved value)
		MyGeometry() : vertexBuffer(0), textureBuffer(0), normalBuffer(0), vertexArray(0), elementCount(0), transformBuffer(0)
		{}
	};


	struct MyFrameBuffer {
		GLuint fbo;
		GLuint texture;
		GLuint rbo;

		MyFrameBuffer() :fbo(0), texture(0), rbo(0) {}
	};


	void bufferGeometry(MyGeometry* geometry, const std::vector<glm::vec3>& pos, const std::vector<glm::vec2>& texcoord, const std::vector<glm::vec3>& nor);
	void initGeometry(MyGeometry* geometry);
	void loadGeometry(MyGeometry* geometry, char* path);
	void Render(MyGeometry *geometry, Material* materal, glm::mat4 transform);
	void RenderInstanced(MyGeometry *geometry, Material* material, glm::mat4 transform);
	bool InitializeShaders(MyShader *shader, const std::string vertex, const std::string fragment);
	bool InitializeFrameBuffer(MyFrameBuffer* frameBuffer, glm::vec2 dimension, bool HDR);
	bool InitializeShadowMap(MyFrameBuffer* frameBuffer, glm::vec2 dimension);
	bool InitializeTexture(MyTexture* texture, const char* filename, GLuint target = GL_TEXTURE_2D,
		GLuint wrap = GL_CLAMP_TO_EDGE, GLuint filter = GL_LINEAR);
}

namespace Viewport {
	extern std::vector<glm::mat4> projection;

	extern std::vector<glm::vec3> position, target;

	void init(int);
	void update(int);
}

namespace Light {
	extern glm::mat4 biasMatrix;

	extern glm::vec3 ambient;

	extern std::vector<glm::mat4> transform, projection;
	extern std::vector<glm::vec3> position, target, direction;

	void init(int);
	void update(int);
	void renderShadowMap(Graphics::MyGeometry* geometry, glm::mat4 obj);
	void renderShadowMapInstanced(Graphics::MyGeometry* geometry, glm::mat4 obj);
	void flushShadowMap();
}

namespace Effects {
	extern float sigma;
	extern int blurSize;
	extern float splitscreenLineThickness;
}