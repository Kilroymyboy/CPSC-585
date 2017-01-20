#include "Graphics.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <vector>

using namespace std;
using namespace glm;

namespace Graphics {

	void QueryGLVersion();
	bool CheckGLErrors();

	string LoadSource(const string &filename);
	GLuint CompileShader(GLenum shaderType, const string &source);
	GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader);

	bool InitializeShaders(MyShader *shader)
	{
		// load shader source from files
		string vertexSource = LoadSource("vertex.glsl");
		string fragmentSource = LoadSource("fragment.glsl");
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

	void RenderScene(MyGeometry *geometry, MyShader *shader)
	{
		// clear screen to a dark grey colour
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// bind our shader program and the vertex array object containing our
		// scene geometry, then tell OpenGL to draw our geometry
		glUseProgram(shader->program);
		glBindVertexArray(geometry->vertexArray);

		Viewport::update();
		Light::update();

		glDrawArrays(GL_TRIANGLES, 0, geometry->elementCount);

		// reset state to default (no shader or geometry bound)
		glBindVertexArray(0);
		glUseProgram(0);

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

	GLFWwindow *window;
	MyShader shader;
	MyGeometry geometry;

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

		// anti aliasing
		glfwWindowHint(GLFW_SAMPLES, 4);

		int width = 1024, height = 1024;
		window = glfwCreateWindow(width, height, "CPSC 453 OpenGL Boilerplate", 0, 0);
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

		// enable gl depth test
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		// call function to load and compile shader programs

		if (!InitializeShaders(&shader)) {
			cout << "Program could not initialize shaders, TERMINATING" << endl;
			return -1;
		}

		// call function to create and fill buffers with geometry data

	//	if (!InitializeGeometry(&geometry))
	//		cout << "Program failed to intialize geometry!" << endl;

		loadGeometry(&geometry, "aventador.obj");

		return 0;
	}



	int shouldClose() {
		return glfwWindowShouldClose(window);
	}

	float lasttime = 0;

	void update() {
		cout << glfwGetTime() - lasttime << endl;
		lasttime = glfwGetTime();

		for (int i = 0; i < 1; i++)
			RenderScene(&geometry, &shader);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	void destroy() {
		DestroyGeometry(&geometry);
		DestroyShaders(&shader);
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
		projection = perspective(PI / 3, 1.0, 0.1, 1000.0);
	}

	void update() {
		transform = lookAt(vec3(cos(glfwGetTime() / 3) * 5, 2, sin(glfwGetTime() / 3) * 5), vec3(0, 0, 0), vec3(0, 1, 0));
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
		direction = vec3(1, -1, -2);
		ambient = vec3(0.2, 0.2, 0.2);
	}

	void update() {
		direction = vec3(cos(glfwGetTime()), -0.8, sin(glfwGetTime()));
		glUniform3f(LIGHT_LOCATION, direction.x, direction.y, direction.z);
		glUniform3f(AMBIENT_LOCATION, ambient.x, ambient.y, ambient.z);
	}
}