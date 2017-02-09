#include "Graphics.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <vector>

using namespace std;
using namespace glm;

namespace Graphics {

	GLFWwindow *window;

	MyFrameBuffer tonemappingFbo;
	MyFrameBuffer msaaFbo;
	MyFrameBuffer aberrationFbo;

	void QueryGLVersion();
	bool CheckGLErrors();

	string LoadSource(const string &filename);
	GLuint CompileShader(GLenum shaderType, const string &source);
	GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader);

	bool InitializeShaders(MyShader *shader, const string vertex, const string fragment)
	{
		// load shader source from files
		string vertexSource = LoadSource(vertex);
		string fragmentSource = LoadSource(fragment);
		if (vertexSource.empty() || fragmentSource.empty()) return false;

		// compile shader source into shader objects
		shader->vertex = CompileShader(GL_VERTEX_SHADER, vertexSource);
		shader->fragment = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

		// link shader program
		shader->program = LinkProgram(shader->vertex, shader->fragment);

		// check for OpenGL errors and return false if error occurred
		return !CheckGLErrors();
	}

	void DestroyShaders(MyShader *shader)
	{
		// unbind any shader programs and destroy shader objects
		glUseProgram(0);
		glDeleteProgram(shader->program);
		glDeleteShader(shader->vertex);
		glDeleteShader(shader->fragment);
	}

	bool InitializeTexture(MyTexture* texture, const char* filename, GLuint target = GL_TEXTURE_2D)
	{
		int numComponents;
		stbi_set_flip_vertically_on_load(true);
		unsigned char *data = stbi_load(filename, &texture->width, &texture->height, &numComponents, 0);
		if (data != nullptr)
		{
			texture->target = target;
			glGenTextures(1, &texture->textureID);
			glBindTexture(texture->target, texture->textureID);
			GLuint format = GL_RGB;
			switch (numComponents)
			{
			case 4:
				format = GL_RGBA;
				break;
			case 3:
				format = GL_RGB;
				break;
			case 2:
				format = GL_RG;
				break;
			case 1:
				format = GL_RED;
				break;
			default:
				cout << "Invalid Texture Format" << endl;
				break;
			};
			glTexImage2D(texture->target, 0, format, texture->width, texture->height, 0, format, GL_UNSIGNED_BYTE, data);

			// Note: Only wrapping modes supported for GL_TEXTURE_RECTANGLE when defining
			// GL_TEXTURE_WRAP are GL_CLAMP_TO_EDGE or GL_CLAMP_TO_BORDER
			glTexParameteri(texture->target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(texture->target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(texture->target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(texture->target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			// Clean up
			glBindTexture(texture->target, 0);
			stbi_image_free(data);
			return !CheckGLErrors();
		}
		return true; //error
	}

	void DestroyTexture(MyTexture *texture)
	{
		glBindTexture(texture->target, 0);
		glDeleteTextures(1, &texture->textureID);
	}
	/*
	bool InitializeGeometry(MyGeometry *geometry)
	{
		// three vertex positions and assocated colours of a triangle
		const GLfloat vertices[][3] = {
			{ -.6f, -.4f,0.0f },
			{ .0f,  .6f,0.0f },
			{ .6f, -.4f,0.0f }
		};

		const GLfloat colours[][3] = {
			{ -1.0f, 0.0f, 0.0f },
			{ -1.0f, 0.0f, 0.0f },
			{ -1.0f, 0.0f, 0.0f }
		};

		geometry->elementCount = 3;

		// create an array buffer object for storing our vertices
		glGenBuffers(1, &geometry->vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// create another one for storing our colours
		glGenBuffers(1, &geometry->normalBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, geometry->normalBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(colours), colours, GL_STATIC_DRAW);

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

		// check for OpenGL errors and return false if error occurred
		return !CheckGLErrors();
	}
	*/
	// deallocate geometry-related objects
	void DestroyGeometry(MyGeometry *geometry)
	{
		// unbind and destroy our vertex array object and associated buffers
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &geometry->vertexArray);
		glDeleteBuffers(1, &geometry->vertexBuffer);
		glDeleteBuffers(1, &geometry->normalBuffer);
	}

	void clearFrameBuffer() {
		glBindFramebuffer(GL_FRAMEBUFFER, tonemappingFbo.fbo);
		glBindTexture(GL_TEXTURE_2D, tonemappingFbo.texture);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, msaaFbo.fbo);
		glBindTexture(GL_TEXTURE_2D, msaaFbo.texture);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void RenderScene(MyGeometry *geometry, MyShader *shader, void(*material)(), mat4 transform)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, tonemappingFbo.fbo);
		glBindTexture(GL_TEXTURE_2D, tonemappingFbo.texture);

		// enable gl depth test
		glEnable(GL_DEPTH_TEST);
		glScissor(0, 0, WINDOW_WIDTH*MSAA, WINDOW_HEIGHT*MSAA);
		glViewport(0, 0, WINDOW_WIDTH*MSAA, WINDOW_HEIGHT*MSAA);

		// bind our shader program and the vertex array object containing our
		// scene geometry, then tell OpenGL to draw our geometry
		glUseProgram(shader->program);
		glBindVertexArray(geometry->vertexArray);

		material();
		Viewport::update(transform);
		Light::update();

		glDrawArrays(GL_TRIANGLES, 0, geometry->elementCount);

		// reset state to default (no shader or geometry bound)
		glBindVertexArray(0);
		glUseProgram(0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		// check for an report any OpenGL errors
		CheckGLErrors();
	}

	void QueryGLVersion()
	{
		// query opengl version and renderer information
		string version = reinterpret_cast<const char *>(glGetString(GL_VERSION));
		string glslver = reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION));
		string renderer = reinterpret_cast<const char *>(glGetString(GL_RENDERER));

		cout << "OpenGL [ " << version << " ] "
			<< "with GLSL [ " << glslver << " ] "
			<< "on renderer [ " << renderer << " ]" << endl;
	}

	bool CheckGLErrors()
	{
		bool error = false;
		for (GLenum flag = glGetError(); flag != GL_NO_ERROR; flag = glGetError())
		{
			cout << "OpenGL ERROR:  ";
			switch (flag) {
			case GL_INVALID_ENUM:
				cout << "GL_INVALID_ENUM" << endl; break;
			case GL_INVALID_VALUE:
				cout << "GL_INVALID_VALUE" << endl; break;
			case GL_INVALID_OPERATION:
				cout << "GL_INVALID_OPERATION" << endl; break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				cout << "GL_INVALID_FRAMEBUFFER_OPERATION" << endl; break;
			case GL_OUT_OF_MEMORY:
				cout << "GL_OUT_OF_MEMORY" << endl; break;
			default:
				cout << "[unknown error code]" << endl;
			}
			error = true;
		}
		return error;
	}

	// --------------------------------------------------------------------------
	// OpenGL shader support functions

	// reads a text file with the given name into a string
	string LoadSource(const string &filename)
	{
		string source;

		ifstream input(filename.c_str());
		if (input) {
			copy(istreambuf_iterator<char>(input),
				istreambuf_iterator<char>(),
				back_inserter(source));
			input.close();
		}
		else {
			cout << "ERROR: Could not load shader source from file "
				<< filename << endl;
		}

		return source;
	}

	// creates and returns a shader object compiled from the given source
	GLuint CompileShader(GLenum shaderType, const string &source)
	{
		// allocate shader object name
		GLuint shaderObject = glCreateShader(shaderType);

		// try compiling the source as a shader of the given type
		const GLchar *source_ptr = source.c_str();
		glShaderSource(shaderObject, 1, &source_ptr, 0);
		glCompileShader(shaderObject);

		// retrieve compile status
		GLint status;
		glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE)
		{
			GLint length;
			glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &length);
			string info(length, ' ');
			glGetShaderInfoLog(shaderObject, info.length(), &length, &info[0]);
			cout << "ERROR compiling shader:" << endl << endl;
			cout << source << endl;
			cout << info << endl;
		}

		return shaderObject;
	}

	// creates and returns a program object linked from vertex and fragment shaders
	GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader)
	{
		// allocate program object name
		GLuint programObject = glCreateProgram();

		// attach provided shader objects to this program
		if (vertexShader)   glAttachShader(programObject, vertexShader);
		if (fragmentShader) glAttachShader(programObject, fragmentShader);

		// try linking the program with given attachments
		glLinkProgram(programObject);

		// retrieve link status
		GLint status;
		glGetProgramiv(programObject, GL_LINK_STATUS, &status);
		if (status == GL_FALSE)
		{
			GLint length;
			glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &length);
			string info(length, ' ');
			glGetProgramInfoLog(programObject, info.length(), &length, &info[0]);
			cout << "ERROR linking shader program:" << endl;
			cout << info << endl;
		}

		return programObject;
	}

	// reports GLFW errors
	void ErrorCallback(int error, const char* description)
	{
		cout << "GLFW ERROR " << error << ":" << endl;
		cout << description << endl;
	}

	// handles keyboard input events
	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
	}

	GLfloat quadVertices[] = {
		-1.0f,  1.0f,  0.0f, 1,
		1.0f,  1.0f,  1, 1,
		1.0f, -1.0f,  1, 0.0f,

		1.0f, -1.0f, 1, 0.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		-1.0f,  1.0f,  0.0f, 1
	};


	int init() {
		// initialize the GLFW windowing system
		if (!glfwInit()) {
			cout << "ERROR: GLFW failed to initialize, TERMINATING" << endl;
			return -1;
		}
		glfwSetErrorCallback(ErrorCallback);

		// attempt to create a window with an OpenGL 4.1 core profile context

		window = 0;
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "CPSC 453 OpenGL Boilerplate", 0, 0);
		if (!window) {
			cout << "Program failed to create GLFW window, TERMINATING" << endl;
			glfwTerminate();
			return -1;
		}

		// set keyboard callback function and make our context current (active)
		glfwSetKeyCallback(window, KeyCallback);
		glfwMakeContextCurrent(window);

		//Intialize GLAD
#ifndef LAB_LINUX
		if (!gladLoadGL())
		{
			cout << "GLAD init failed" << endl;
			return -1;
		}
#endif

		// query and print out information about our OpenGL environment
		QueryGLVersion();

		// frame buffer objects
		if (!InitializeFrameBuffer(&tonemappingFbo, "tonemapping.glsl", vec2(WINDOW_WIDTH*MSAA, WINDOW_HEIGHT*MSAA), 1)) {
			return -1;
		}

		if (!InitializeFrameBuffer(&msaaFbo, "downsample.glsl", vec2(WINDOW_WIDTH*MSAA, WINDOW_HEIGHT*MSAA), 0)) {
			return -1;
		}

		if (!InitializeFrameBuffer(&aberrationFbo, "aberration.glsl", vec2(WINDOW_WIDTH, WINDOW_HEIGHT), 0)) {
			return -1;
		}

		return 0;
	}

	bool InitializeFrameBuffer(MyFrameBuffer* frameBuffer, const string &fragment, vec2 dimension, bool HDR) {
		glGenFramebuffers(1, &frameBuffer->fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer->fbo);

		glGenTextures(1, &frameBuffer->texture);
		glBindTexture(GL_TEXTURE_2D, frameBuffer->texture);

		glTexImage2D(GL_TEXTURE_2D, 0, HDR ? GL_RGBA16F : GL_RGBA, dimension.x, dimension.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glGenRenderbuffers(1, &frameBuffer->rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, frameBuffer->rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, dimension.x, dimension.y);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, frameBuffer->rbo);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameBuffer->texture, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			cout << "Frame buffer is dead" << endl;
			return false;
		}

		if (!InitializeShaders(&frameBuffer->shader, "framebuffervertex.glsl", fragment)) {
			cout << "Program could not initialize framebuffer shaders, TERMINATING" << endl;
			return false;
		}

		glUseProgram(frameBuffer->shader.program);
		glGenVertexArrays(1, &frameBuffer->vao);
		glGenBuffers(1, &frameBuffer->vbo);
		glBindBuffer(GL_ARRAY_BUFFER, frameBuffer->vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
		glBindVertexArray(frameBuffer->vao);
		glBindBuffer(GL_ARRAY_BUFFER, frameBuffer->vbo);
		glUniform1i(glGetUniformLocation(frameBuffer->shader.program, "texFramebuffer"), 0);

		GLint posAttrib = glGetAttribLocation(frameBuffer->shader.program, "position");
		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

		GLint texAttrib = glGetAttribLocation(frameBuffer->shader.program, "texcoord");
		glEnableVertexAttribArray(texAttrib);
		glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

		return true;
	}

	int shouldClose() {
		return glfwWindowShouldClose(window);
	}

	void renderTonemapping() {
		glBindFramebuffer(GL_FRAMEBUFFER, msaaFbo.fbo);
		glBindTexture(GL_TEXTURE_2D, msaaFbo.texture);

		glScissor(0, 0, WINDOW_WIDTH*MSAA, WINDOW_HEIGHT*MSAA);
		glViewport(0, 0, WINDOW_WIDTH*MSAA, WINDOW_HEIGHT*MSAA);

		glBindVertexArray(tonemappingFbo.vao);
		glDisable(GL_DEPTH_TEST);
		glUseProgram(tonemappingFbo.shader.program);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tonemappingFbo.texture);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void renderMSAA() {
		glBindFramebuffer(GL_FRAMEBUFFER, aberrationFbo.fbo);
		glBindTexture(GL_TEXTURE_2D, aberrationFbo.texture);

		glScissor(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

		glBindVertexArray(msaaFbo.vao);
		glDisable(GL_DEPTH_TEST);
		glUseProgram(msaaFbo.shader.program);

		glUniform2f(1, WINDOW_WIDTH*MSAA, WINDOW_HEIGHT*MSAA);
		glUniform1i(2, MSAA);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, msaaFbo.texture);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void renderAberration() {
		glScissor(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

		glBindVertexArray(aberrationFbo.vao);
		glDisable(GL_DEPTH_TEST);
		glUseProgram(aberrationFbo.shader.program);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, aberrationFbo.texture);

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void update() {
		renderTonemapping();
		renderMSAA();
		renderAberration();

		CheckGLErrors();

		// vertical sync
		glfwSwapInterval(VSYNC);
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	void destroy() {
		glDeleteFramebuffers(1, &tonemappingFbo.fbo);
		glDeleteRenderbuffers(1, &tonemappingFbo.rbo);
		glfwDestroyWindow(window);
		glfwTerminate();

		cout << "Goodbye!" << endl;
	}

	void loadGeometry(MyGeometry* geometry, char* path) {
		vector<vec3> vertices, normals, bufferVertices, bufferNormals;
		FILE * file = fopen(path, "r");
		int asdf = 0;
		while (1) {
			char lineHeader[128];
			int res = fscanf(file, "%s", lineHeader);
			if (res == EOF)break;
			if (strcmp(lineHeader, "v") == 0) {
				vec3 vertex;
				fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				vertices.push_back(vertex);
			}
			else if (strcmp(lineHeader, "vn") == 0) {
				vec3 normal;
				fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				normals.push_back(normal);
			}
			else if (strcmp(lineHeader, "f") == 0) {
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
				if (matches != 9) {
					printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				}
				ivec3 tv(vertexIndex[0] - 1, vertexIndex[1] - 1, vertexIndex[2] - 1);
				bufferVertices.push_back(vertices[tv.x]);
				bufferVertices.push_back(vertices[tv.y]);
				bufferVertices.push_back(vertices[tv.z]);
				ivec3 tn(normalIndex[0] - 1, normalIndex[1] - 1, normalIndex[2] - 1);
				bufferNormals.push_back(normals[tn.x]);
				bufferNormals.push_back(normals[tn.y]);
				bufferNormals.push_back(normals[tn.z]);
			}
		}

		geometry->elementCount = bufferVertices.size();
		cout << geometry->elementCount << endl;

		glGenBuffers(1, &geometry->vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*bufferVertices.size(), &bufferVertices[0], GL_STATIC_DRAW);

		// create another one for storing our colours
		glGenBuffers(1, &geometry->normalBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, geometry->normalBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*bufferNormals.size(), &bufferNormals[0], GL_STATIC_DRAW);

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

}

namespace Viewport {
	glm::mat4 transform;
	glm::mat4 projection;

	void init() {
		transform = lookAt(vec3(5, 2, 5), vec3(0, 0, 0), vec3(0, 1, 0));
		projection = perspective(PI / 3, (double)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1, 1000.0);
	}

	void update(mat4 obj) {
		transform = lookAt(vec3(cos(6 / 1.5f) * 4.0f, 1, sin(6 / 1.5f) * 4.0f), vec3(0, 0, 0), vec3(0, 1, 0))*obj;
			transform = lookAt(vec3(cos(glfwGetTime() / 1.5f) * 4.5f, 1, sin(glfwGetTime() / 1.5f) * 4.5f), vec3(0, 0, 0), vec3(0, 1, 0))*obj;
			//	transform = lookAt(vec3(0, 2, -6.5f), vec3(0, 2, 0), vec3(0, 1, 0))*obj;
		glUniformMatrix4fv(MODELVIEW_LOCATION, 1, false, &transform[0][0]);
		glUniformMatrix4fv(PROJECTION_LOCATION, 1, false, &projection[0][0]);
	}
}

namespace Light {
	glm::vec3 color;
	glm::vec3 direction;
	glm::vec3 ambient;

	void init() {
		color = vec3(.1f, .1f, .1f);
		direction = vec3(0, -1, 0);
		ambient = vec3(0.05, 0.05, 0.05);
	}

	void update() {
		direction = vec3(cos(glfwGetTime()), -0.8, sin(glfwGetTime()));
		glUniform3f(LIGHT_LOCATION, direction.x, direction.y, direction.z);
		glUniform3f(AMBIENT_LOCATION, ambient.x, ambient.y, ambient.z);
	}
}
