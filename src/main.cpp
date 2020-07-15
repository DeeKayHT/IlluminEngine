
#include <stdio.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"

#include "main.h"
#include "imgui_game.h"
#include "input.h"
#include "render.h"

const int DEFAULT_SCREEN_X = 1600;
const int DEFAULT_SCREEN_Y = 900;


// ----------------------------------------------------
// Local Variables
// ----------------------------------------------------

static SDL_Window* gSDL_window = nullptr;
static bool gQuit = false;


// ----------------------------------------------------
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

	// Setup Dear ImGui context
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(gSDL_window, &context);
	ImGui_ImplOpenGL3_Init(NULL);
	// ImGui style setup
	ImGui::StyleColorsClassic();

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


void Game_SetQuitStatus(bool quit)	{ gQuit |= quit; }	// Always quit if set
SDL_Window* GetWindow()				{ return gSDL_window; }


int main( int argc, char* argv[] )
{
	if (Init() < 0) {
		Shutdown();
		return -1;
	}

	HardwareInfo();
	Render_Setup();
	
	while (!gQuit) {
		Input_Process();
		Render();
		ImGui_Loop();

		// Swap buffers
		SDL_GL_SwapWindow(gSDL_window);
	}
	
	Shutdown();
	return 0;
}
