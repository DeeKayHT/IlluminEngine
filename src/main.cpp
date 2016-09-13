
#include <stdio.h>

#include "main.h"
#include "render.h"

const int DEFAULT_SCREEN_X = 1280;
const int DEFAULT_SCREEN_Y = 720;

static SDL_Window* gSDL_window = nullptr;
static bool gQuit = false;

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

static void HardwareInfo()
{
	GLint attributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &attributes);
	printf("Max %i vertex attributes\n", attributes);
}

static void Shutdown()
{
	Render_Shutdown();
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

SDL_Window* GetWindow() { return gSDL_window; }


int main( int argc, char* argv[] )
{
	if (Init() < 0) {
		Shutdown();
		return -1;
	}

	HardwareInfo();
	Render_Setup();
	
	while (!gQuit) {
		ProcessEvents();
		Render();
	}
	
	Shutdown();
	return 0;
}
