#pragma once

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <iterator>
#include <string>
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
#define MODELVIEW_LOCATION 3
#define LIGHT_LOCATION 4
#define AMBIENT_LOCATION 5
#define COLOR_LOCATION 6
#define EMISSION_COLOR_LOCATION 7

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 1024

namespace Graphics {
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

	void clearFrameBuffer();
	void loadGeometry(MyGeometry* geometry, char* path);
	void RenderScene(MyGeometry *geometry, MyShader *shader, void(*material)());
	bool InitializeShaders(MyShader *shader, const std::string vertex, const std::string fragment);
}

namespace Viewport {
	extern glm::mat4 transform;
	extern glm::mat4 projection;

	void init();
	void update();
}

namespace Light {
	extern glm::vec3 color;
	extern glm::vec3 direction;
	extern glm::vec3 ambient;

	void init();
	void update();
}