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
#define PROJECTION_LOCATION 2
#define VIEW_LOCATION 3
#define MODEL_LOCATION 4
#define LIGHT_LOCATION 5
#define AMBIENT_LOCATION 6
#define COLOR_LOCATION 7
#define EMISSION_COLOR_LOCATION 8
#define SHADOW_MVP_LOCATION 9

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define MSAA 2
#define VSYNC 1
#define SHADOWMAP_SIZE 1600

#define EFFECTS 0
#define HDR_ENABLED 0

namespace Graphics {
	extern bool SPLIT_SCREEN;
	// 0 horizontal/side by side, 1 vertical/stacked
	extern int SPLIT_SCREEN_ORIENTATION;

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

	struct MyGeometry
	{
		// OpenGL names for array buffer objects, vertex array object
		GLuint  vertexBuffer;
		GLuint  textureBuffer;
		GLuint  normalBuffer;
		GLuint  vertexArray;
		GLsizei elementCount;

		// initialize object names to zero (OpenGL reserved value)
		MyGeometry() : vertexBuffer(0), normalBuffer(0), vertexArray(0), elementCount(0)
		{}
	};

	struct MyFrameBuffer {
		GLuint fbo;
		GLuint texture;
		GLuint rbo;

		MyFrameBuffer() :fbo(0), texture(0), rbo(0) {}
	};

	void loadGeometry(MyGeometry* geometry, char* path);
	void Render(MyGeometry *geometry, void(*material)(), glm::mat4 transform);
	bool InitializeShaders(MyShader *shader, const std::string vertex, const std::string fragment);
	bool InitializeFrameBuffer(MyFrameBuffer* frameBuffer, glm::vec2 dimension, bool HDR);
	bool InitializeShadowMap(MyFrameBuffer* frameBuffer, glm::vec2 dimension);
}

namespace Viewport {
	extern std::vector<glm::mat4> projection;

	extern std::vector<glm::vec3> position, target;

	void init(int);
	void update(glm::mat4);
}

namespace Light {
	extern glm::mat4 biasMatrix;

	extern glm::vec3 color;
	extern glm::vec3 direction;
	extern glm::vec3 ambient;

	extern glm::mat4 transform;
	extern glm::mat4 projection;
	extern glm::vec3 position, target;

	void init();
	void update();
	void renderShadowMap(Graphics::MyGeometry* geometry, glm::mat4 obj);
}

namespace Effects {
	extern float sigma;
	extern float splitscreenLineThickness;
}