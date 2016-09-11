#define GLEW_STATIC

#include <stdio.h>

#include "GL/glew.h"
#include "SDL/SDL.h"

const int DEFAULT_SCREEN_X = 1280;
const int DEFAULT_SCREEN_Y = 720;

static SDL_Window* gSDL_window = nullptr;
static bool gQuit = false;
static GLuint gCurShaderProgram = 0;
static GLuint gVAO = 0;

static int Init()
{
	uint32_t initFlags = SDL_INIT_VIDEO;
	if (SDL_Init(initFlags) < 0) {
		return -1;
	}

	// Use OpenGL 3.3 Core
	// Must be done before window creation!
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	uint32_t windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
	gSDL_window = SDL_CreateWindow("IlluminEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, DEFAULT_SCREEN_X, DEFAULT_SCREEN_Y, windowFlags);
	if (gSDL_window == NULL) {
		return -2;
	}

	SDL_GLContext context = SDL_GL_CreateContext(gSDL_window);
	if (context == nullptr) {
		return -3;
	}

	// GLEW init must be done after GL Context creation!
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		return -4;
	}

	return 0;
}

static void Shutdown()
{
	if (gSDL_window != nullptr) {
		SDL_DestroyWindow(gSDL_window);
	}

	SDL_Quit();
}

static void ProcessEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event) != 0) {
		if (event.type == SDL_QUIT) {
			gQuit = true;
		}
	}
}


static void RenderSetup()
{
	// Setup Viewport
	int width, height;	// Resolution (TODO: Get resolution from data)
	SDL_GetWindowSize(gSDL_window, &width, &height);
	glViewport(0, 0, width, height);
	
	SDL_GL_SetSwapInterval(1);	// Set vsync for now to avoid running hardware to max


	// TODO: Create file loader!
	SDL_RWops* vertFile = SDL_RWFromFile("..\\data\\shaders\\basic.vert", "r");
	size_t vertFileSize = static_cast<size_t>( SDL_RWsize( vertFile ) );
	void* vertShaderBuffer = malloc(vertFileSize);
	SDL_RWread(vertFile, vertShaderBuffer, vertFileSize, 1);

	// Load and compile the basic vertex shader
	// TODO: Use proper types, not C type-casting!
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, (GLchar**)&vertShaderBuffer, (GLint*)&vertFileSize);
	glCompileShader(vertexShader);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, sizeof(infoLog), NULL, infoLog);
		printf("%s\n", infoLog);
	}


	// TODO: Create file loader!
	SDL_RWops* fragFile = SDL_RWFromFile("..\\data\\shaders\\basic.frag", "r");
	size_t fragFileSize = static_cast<size_t>(SDL_RWsize(fragFile));
	void* fragShaderBuffer = malloc(fragFileSize);
	SDL_RWread(fragFile, fragShaderBuffer, fragFileSize, 1);

	// Load and compile the basic fragment shader
	// TODO: Use proper types, not C type-casting!
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, (GLchar**)&fragShaderBuffer, (GLint*)&fragFileSize);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, sizeof(infoLog), NULL, infoLog);
		printf("%s\n", infoLog);
	}

	// Attach the shaders to a program to be used
	gCurShaderProgram = glCreateProgram();
	glAttachShader(gCurShaderProgram, vertexShader);
	glAttachShader(gCurShaderProgram, fragmentShader);
	glLinkProgram(gCurShaderProgram);

	glGetProgramiv(gCurShaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(gCurShaderProgram, sizeof(infoLog), NULL, infoLog);
		printf("%s\n", infoLog);
	}

	// The shaders are no longer needed once linked to a program
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	// Setup basic rectangle
	GLfloat vertices[] = {
		0.5f, 0.5f, 0.0f,	// Top Right
		0.5f, -0.5f, 0.0f,	// Bottom Right
		-0.5f, -0.5f, 0.0f,	// Bottom Left
		-0.5f,  0.5f, 0.0f,	// Top Left
	};
	GLuint indices[] = {
		0, 1, 3,	// Triangle 1
		1, 2, 3,	// Triangle 2
	};

	// Create a VAO to store vertex attribute data with VBOs
	glGenVertexArrays(1, &gVAO);
	glBindVertexArray(gVAO);	// Subsequent OpenGL calls with VBOs and vertex attributes will affect this VAO

	// Create a vertex buffer object (VBO) to send
	// vertex data to the GPU as a single data stream
	GLuint VBO;
	glGenBuffers(1, &VBO);				// Create one VBO and store it in variable VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);	// Bind VBO to the GL_ARRAY_BUFFER target
	// Now send the vertex data to the VBO target
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Explanation:
	// - 1st parameter is which the index to send the data to. In basic.vert shader this is (location = 0)
	// - 2nd parameter is the size of each vertex attribute. It's a vec3 (for position) so it has 3 values.
	// - 3rd parameter is the type of the data, which are floats.
	// - 4th parameter sets whether the data should be normalized.
	// - 5th parameter is stride between each vertex's data. Data only has position, which is 3 floats.
	// - Last parameter is offset where this data begins. It is set to 0 because there is no offset.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Unbind the VBO and VAO to prevent any subsequent OpenGL calls from potentially modifying it
	glBindBuffer(GL_ARRAY_BUFFER, 0);	// Okay to unbind VBO since vertexAttribute registered the VBO
	glBindVertexArray(0);
}

static void Render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(gCurShaderProgram);
	glBindVertexArray(gVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// Swap buffers
	SDL_GL_SwapWindow(gSDL_window);
}

int main( int argc, char* argv[] )
{
	if (Init() < 0) {
		Shutdown();
		return -1;
	}

	RenderSetup();
	
	while (!gQuit) {
		ProcessEvents();
		Render();
	}
	
	Shutdown();
	return 0;
}
