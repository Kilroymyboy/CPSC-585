#include "Graphics.h"
#include "InputManager.h"
#include "Game.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


using namespace std;
using namespace glm;

namespace Graphics {

	GLFWwindow *window;

	MyFrameBuffer defaultFbo;
	MyFrameBuffer defaultFbo1;

	MyFrameBuffer splitScreenFbo;

	MyFrameBuffer tonemappingFbo;
	MyFrameBuffer msaaFbo;
	MyFrameBuffer aberrationFbo;

	MyFrameBuffer shadowFbo;

	MyFrameBuffer hBlurFbo;
	MyFrameBuffer vBlurFbo;
	MyFrameBuffer additiveFbo;

	MyShader tonemappingShader;
	MyShader msaaShader;
	MyShader aberrationShader;
	MyShader blurShader;
	MyShader additiveShader;
	MyShader shadowmapShader;

	MyShader splitscreenShader;

	GLuint frameBufferVao;

	bool SPLIT_SCREEN = 0;
	// 0 horizontal/side by side, 1 vertical/stacked
	int SPLIT_SCREEN_ORIENTATION = 0;

	void QueryGLVersion();
	bool CheckGLErrors();

	string LoadSource(const string &filename);
	GLuint CompileShader(GLenum shaderType, const string &source);
	GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader);

	void InitializeFrameBufferVAO();

	bool InitializeShaders(MyShader *shader, const string vertex, const string fragment)
	{
		// load shader source from files
		string vertexSource = LoadSource("shaders/" + vertex);
		string fragmentSource = LoadSource("shaders/" + fragment);
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
		glBindFramebuffer(GL_FRAMEBUFFER, defaultFbo.fbo);
		glBindTexture(GL_TEXTURE_2D, defaultFbo.texture);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, shadowFbo.fbo);
		glBindTexture(GL_TEXTURE_2D, shadowFbo.texture);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, msaaFbo.fbo);
		glBindTexture(GL_TEXTURE_2D, msaaFbo.texture);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Render(MyGeometry *geometry, void(*material)(), mat4 transform)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, defaultFbo.fbo);
		glBindTexture(GL_TEXTURE_2D, defaultFbo.texture);

		// enable gl depth test
		glEnable(GL_DEPTH_TEST);
		if (SPLIT_SCREEN) {
			vec2 defaultFboDimension(WINDOW_WIDTH*MSAA / ((SPLIT_SCREEN && (!SPLIT_SCREEN_ORIENTATION)) ? 2 : 1),
				WINDOW_HEIGHT*MSAA / ((SPLIT_SCREEN && SPLIT_SCREEN_ORIENTATION) ? 2 : 1));

			glScissor(0, 0, defaultFboDimension.x, defaultFboDimension.y);
			glViewport(0, 0, defaultFboDimension.x, defaultFboDimension.y);
		}
		else {
			glScissor(0, 0, WINDOW_WIDTH*MSAA, WINDOW_HEIGHT*MSAA);
			glViewport(0, 0, WINDOW_WIDTH*MSAA, WINDOW_HEIGHT*MSAA);
		}

		glBindVertexArray(geometry->vertexArray);

		material();
		Viewport::update(transform);
		Light::update();

		mat4 shadowMvp = Light::biasMatrix*Light::projection*Light::transform;
		glUniformMatrix4fv(SHADOW_MVP_LOCATION, 1, GL_FALSE, &shadowMvp[0][0]);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, shadowFbo.texture);

		glDrawArrays(GL_TRIANGLES, 0, geometry->elementCount);

		// reset state to default (no shader or geometry bound)
		glBindVertexArray(0);
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

		InputManager::keyBoard(key, 0, 0);
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
		glfwSetKeyCallback(window, Keyboard::Callback);
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
		vec2 defaultFboDimension(WINDOW_WIDTH*MSAA / ((SPLIT_SCREEN && (!SPLIT_SCREEN_ORIENTATION)) ? 2 : 1),
			WINDOW_HEIGHT*MSAA / ((SPLIT_SCREEN && SPLIT_SCREEN_ORIENTATION) ? 2 : 1));
		if (!InitializeFrameBuffer(&defaultFbo, defaultFboDimension, 1)) return -1;
		if (!InitializeFrameBuffer(&defaultFbo1, defaultFboDimension, 1)) return -1;
		if (!InitializeFrameBuffer(&splitScreenFbo, vec2(WINDOW_WIDTH *MSAA, WINDOW_HEIGHT *MSAA), 1))	return -1;
		if (!InitializeFrameBuffer(&tonemappingFbo, vec2(WINDOW_WIDTH*MSAA, WINDOW_HEIGHT*MSAA), 1)) return -1;
		if (!InitializeFrameBuffer(&msaaFbo, vec2(WINDOW_WIDTH*MSAA, WINDOW_HEIGHT*MSAA), 0)) return -1;
		if (!InitializeFrameBuffer(&aberrationFbo, vec2(WINDOW_WIDTH, WINDOW_HEIGHT), 0)) return -1;
		if (!InitializeShadowMap(&shadowFbo, vec2(SHADOWMAP_SIZE, SHADOWMAP_SIZE))) return -1;
		if (!InitializeFrameBuffer(&hBlurFbo, vec2(WINDOW_WIDTH, WINDOW_HEIGHT), 1)) return -1;
		if (!InitializeFrameBuffer(&vBlurFbo, vec2(WINDOW_WIDTH, WINDOW_HEIGHT), 1)) return -1;
		if (!InitializeFrameBuffer(&additiveFbo, vec2(WINDOW_WIDTH *MSAA, WINDOW_HEIGHT *MSAA), 1))	return -1;

		if (!InitializeShaders(&tonemappingShader, "framebuffervertex.glsl", "tonemapping.glsl")) return -1;
		if (!InitializeShaders(&msaaShader, "framebuffervertex.glsl", "downsample.glsl")) return -1;
		if (!InitializeShaders(&aberrationShader, "framebuffervertex.glsl", "aberration.glsl")) return -1;
		if (!InitializeShaders(&blurShader, "framebuffervertex.glsl", "blur.glsl")) return -1;
		if (!InitializeShaders(&additiveShader, "framebuffervertex.glsl", "additive.glsl")) return -1;
		if (!InitializeShaders(&shadowmapShader, "shadowmapvertex.glsl", "shadowmapfragment.glsl")) return -1;
		if (!InitializeShaders(&splitscreenShader, "framebuffervertex.glsl", "split.glsl")) return -1;

		InitializeFrameBufferVAO();

		return 0;
	}

	void InitializeFrameBufferVAO() {
		GLuint frameBufferVbo;
		glGenVertexArrays(1, &frameBufferVao);
		glGenBuffers(1, &frameBufferVbo);
		glBindBuffer(GL_ARRAY_BUFFER, frameBufferVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
		glBindVertexArray(frameBufferVao);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
	}

	bool InitializeFrameBuffer(MyFrameBuffer* frameBuffer, vec2 dimension, bool HDR) {
		glGenFramebuffers(1, &frameBuffer->fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer->fbo);

		glGenTextures(1, &frameBuffer->texture);
		glBindTexture(GL_TEXTURE_2D, frameBuffer->texture);

		glTexImage2D(GL_TEXTURE_2D, 0, (HDR&&HDR_ENABLED) ? GL_RGBA16F : GL_RGBA, dimension.x, dimension.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glGenRenderbuffers(1, &frameBuffer->rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, frameBuffer->rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, dimension.x, dimension.y);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, frameBuffer->rbo);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameBuffer->texture, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			cout << "Frame buffer is dead" << endl;
			return false;
		}

		return true;
	}

	bool InitializeShadowMap(MyFrameBuffer* frameBuffer, vec2 dimension) {
		glGenFramebuffers(1, &frameBuffer->fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer->fbo);

		glGenTextures(1, &frameBuffer->texture);
		glBindTexture(GL_TEXTURE_2D, frameBuffer->texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, SHADOWMAP_SIZE, SHADOWMAP_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, frameBuffer->texture, 0);

		glDrawBuffer(GL_NONE);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			return false;
		return true;
	}

	int shouldClose() {
		return glfwWindowShouldClose(window);
	}

	void renderSplitScreen() {
		if (!SPLIT_SCREEN)return;

		glBindFramebuffer(GL_FRAMEBUFFER, splitScreenFbo.fbo);
		glBindTexture(GL_TEXTURE_2D, splitScreenFbo.texture);

		glScissor(0, 0, WINDOW_WIDTH*MSAA, WINDOW_HEIGHT*MSAA);
		glViewport(0, 0, WINDOW_WIDTH*MSAA, WINDOW_HEIGHT*MSAA);

		glBindVertexArray(frameBufferVao);
		glDisable(GL_DEPTH_TEST);
		glUseProgram(splitscreenShader.program);
		glUniform1i(glGetUniformLocation(splitscreenShader.program, "tex0"), 0);
		glUniform1i(glGetUniformLocation(splitscreenShader.program, "tex1"), 1);
		glUniform1i(0, SPLIT_SCREEN_ORIENTATION);
		glUniform1f(1, Effects::splitscreenLineThickness);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, defaultFbo.texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, defaultFbo.texture);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void renderHBlur() {
		glBindFramebuffer(GL_FRAMEBUFFER, vBlurFbo.fbo);
		glBindTexture(GL_TEXTURE_2D, vBlurFbo.texture);

		glScissor(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

		glBindVertexArray(frameBufferVao);
		glDisable(GL_DEPTH_TEST);
		glUseProgram(blurShader.program);
		glUniform1i(glGetUniformLocation(blurShader.program, "texFramebuffer"), 0);

		glUniform2f(0, 1.0f / (WINDOW_WIDTH), 0.00f);
		glUniform1f(1, 1.0f);
		glUniform1f(2, Effects::sigma);

		glActiveTexture(GL_TEXTURE0);
		if (SPLIT_SCREEN)
			glBindTexture(GL_TEXTURE_2D, splitScreenFbo.texture);
		else
			glBindTexture(GL_TEXTURE_2D, defaultFbo.texture);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void renderVBlur() {
		glBindFramebuffer(GL_FRAMEBUFFER, hBlurFbo.fbo);
		glBindTexture(GL_TEXTURE_2D, hBlurFbo.texture);

		glScissor(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

		glBindVertexArray(frameBufferVao);
		glDisable(GL_DEPTH_TEST);
		glUseProgram(blurShader.program);
		glUniform1i(glGetUniformLocation(blurShader.program, "texFramebuffer"), 0);

		glUniform2f(0, 0.0f, 1.0f / (WINDOW_WIDTH));
		glUniform1f(1, 0.0f);
		glUniform1f(2, Effects::sigma);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, vBlurFbo.texture);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

	void renderAdditive() {
		glBindFramebuffer(GL_FRAMEBUFFER, tonemappingFbo.fbo);
		glBindTexture(GL_TEXTURE_2D, tonemappingFbo.texture);

		glScissor(0, 0, WINDOW_WIDTH*MSAA, WINDOW_HEIGHT*MSAA);
		glViewport(0, 0, WINDOW_WIDTH*MSAA, WINDOW_HEIGHT*MSAA);

		glBindVertexArray(frameBufferVao);
		glDisable(GL_DEPTH_TEST);
		glUseProgram(additiveShader.program);
		glUniform1i(glGetUniformLocation(additiveShader.program, "tex0"), 0);
		glUniform1i(glGetUniformLocation(additiveShader.program, "tex1"), 1);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, hBlurFbo.texture);
		glActiveTexture(GL_TEXTURE1);
		if (SPLIT_SCREEN)
			glBindTexture(GL_TEXTURE_2D, splitScreenFbo.texture);
		else
			glBindTexture(GL_TEXTURE_2D, defaultFbo.texture);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void renderTonemapping() {
		glBindFramebuffer(GL_FRAMEBUFFER, msaaFbo.fbo);
		glBindTexture(GL_TEXTURE_2D, msaaFbo.texture);

		glScissor(0, 0, WINDOW_WIDTH*MSAA, WINDOW_HEIGHT*MSAA);
		glViewport(0, 0, WINDOW_WIDTH*MSAA, WINDOW_HEIGHT*MSAA);

		glBindVertexArray(frameBufferVao);
		glDisable(GL_DEPTH_TEST);
		glUseProgram(tonemappingShader.program);
		glUniform1i(glGetUniformLocation(tonemappingShader.program, "texFramebuffer"), 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tonemappingFbo.texture);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void renderMSAA() {
		if (EFFECTS) {
			glBindFramebuffer(GL_FRAMEBUFFER, aberrationFbo.fbo);
			glBindTexture(GL_TEXTURE_2D, aberrationFbo.texture);
		}
		else {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		glScissor(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

		glBindVertexArray(frameBufferVao);
		glDisable(GL_DEPTH_TEST);
		glUseProgram(msaaShader.program);
		glUniform1i(glGetUniformLocation(msaaShader.program, "texFramebuffer"), 0);

		glUniform2f(1, WINDOW_WIDTH*MSAA, WINDOW_HEIGHT*MSAA);
		glUniform1i(2, MSAA);

		glActiveTexture(GL_TEXTURE0);
		if (EFFECTS)
			glBindTexture(GL_TEXTURE_2D, msaaFbo.texture);
		else
			if (SPLIT_SCREEN)
				glBindTexture(GL_TEXTURE_2D, splitScreenFbo.texture);
			else
				glBindTexture(GL_TEXTURE_2D, defaultFbo.texture);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void renderAberration() {
		glScissor(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

		glBindVertexArray(frameBufferVao);
		glDisable(GL_DEPTH_TEST);
		glUseProgram(aberrationShader.program);
		glUniform1i(glGetUniformLocation(aberrationShader.program, "texFramebuffer"), 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, aberrationFbo.texture);

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void update() {
		// clear frame buffer
		clearFrameBuffer();

		// render shadowmap
		for (auto it = Game::entities.begin(); it != Game::entities.end(); it++) {
			it->get()->renderShadowMap(mat4(1));
		}

		// render geometry
		glUseProgram(Resources::standardShader.program);
		for (auto it = Game::entities.begin(); it != Game::entities.end(); it++) {
			it->get()->render(mat4(1));
		}
		glUseProgram(0);

		// render split screen
		renderSplitScreen();

		// render post processing
		if (EFFECTS) {
			renderHBlur();
			renderVBlur();
			renderAdditive();
			renderTonemapping();
		}
		renderMSAA();
		if (EFFECTS) {
			renderAberration();
		}

		CheckGLErrors();

		// vertical sync
		glfwSwapInterval(VSYNC);
		glfwSwapBuffers(window);
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
	std::vector<glm::mat4> transform;
	std::vector<glm::mat4> projection;

	std::vector<glm::vec3> position, target;

	int SIZE;

	void init(int n) {
		SIZE = n;

		transform.resize(SIZE);
		projection.resize(SIZE);
		position.resize(SIZE);
		target.resize(SIZE);

		for (int i = 0; i < SIZE; i++)
			transform[i] = lookAt(vec3(5, 2, 5), vec3(0, 0, 0), vec3(0, 1, 0));
	}

	void update(mat4 obj) {
		double splitscreenRatio = Graphics::SPLIT_SCREEN ? (Graphics::SPLIT_SCREEN_ORIENTATION ? 2 : 0.5) : 1;
		projection[0] = perspective(PI / 3, (double)WINDOW_WIDTH / WINDOW_HEIGHT * splitscreenRatio, 0.1, 1000.0);
		transform[0] = lookAt(position[0], target[0], vec3(0, 1, 0));
		glUniformMatrix4fv(MODEL_LOCATION, 1, false, &obj[0][0]);
		glUniformMatrix4fv(VIEW_LOCATION, 1, false, &transform[0][0][0]);
		glUniformMatrix4fv(PROJECTION_LOCATION, 1, false, &projection[0][0][0]);
	}
}

namespace Light {
	glm::mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);

	glm::vec3 color;
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::mat4 transform;
	glm::mat4 projection;

	glm::vec3 position, target;

	void init() {
		color = vec3(.1f, .1f, .1f);
		direction = vec3(0, -1, 0);
		ambient = vec3(0.05, 0.05, 0.05);
		projection = ortho<float>(-5, 5, -5, 5, -5, 30);
	}

	void update() {
		transform = lookAt(position, target, vec3(0, 1, 0));
		direction = normalize(target - position);
		glUniform3f(LIGHT_LOCATION, direction.x, direction.y, direction.z);
		glUniform3f(AMBIENT_LOCATION, ambient.x, ambient.y, ambient.z);
	}

	void renderShadowMap(Graphics::MyGeometry* geometry, mat4 obj) {
		glBindFramebuffer(GL_FRAMEBUFFER, Graphics::shadowFbo.fbo);
		glBindTexture(GL_TEXTURE_2D, Graphics::shadowFbo.texture);

		// enable gl depth test
		glEnable(GL_DEPTH_TEST);
		glScissor(0, 0, SHADOWMAP_SIZE, SHADOWMAP_SIZE);
		glViewport(0, 0, SHADOWMAP_SIZE, SHADOWMAP_SIZE);

		// bind our shader program and the vertex array object containing our
		// scene geometry, then tell OpenGL to draw our geometry
		glUseProgram(Graphics::shadowmapShader.program);
		glBindVertexArray(geometry->vertexArray);

		transform = lookAt(position, target, vec3(0, 1, 0));
		direction = normalize(target - position);
		mat4 mvp = projection*transform*obj;
		glUniformMatrix4fv(1, 1, GL_FALSE, &mvp[0][0]);

		glDrawArrays(GL_TRIANGLES, 0, geometry->elementCount);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

namespace Effects {
	float sigma = 15;
	float splitscreenLineThickness = 2.0f / WINDOW_WIDTH;
}