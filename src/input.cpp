#include "SDL/SDL.h"

#include "camera.h"

// ----------------------------------------------------
// Global Variables
// ----------------------------------------------------

extern Camera gCamera;


// ----------------------------------------------------
// Local Variables
// ----------------------------------------------------

static bool gEventQuit = false;
static bool gKeyPressed[SDL_NUM_SCANCODES] = { 0 };

// Note: Scancodes are the physical location of the key on the keyboard, while
//       keycodes are soft-keys mapped to the location and are dependent on locale.
//       Keycodes should be used for character-dependent situations.


// ----------------------------------------------------

bool Input_GetQuitStatus() { return gEventQuit; }

void Input_Process()
{
	SDL_Event event;
	while (SDL_PollEvent(&event) != 0) {
		if (event.type == SDL_QUIT) {
			gEventQuit = true;
		} else if (event.type == SDL_KEYDOWN) {
			gKeyPressed[event.key.keysym.scancode] = true;
			
		} else if (event.type == SDL_KEYUP) {
			gKeyPressed[event.key.keysym.scancode] = false;
		}
	}
	// Have Esc also quit!
	if (gKeyPressed[SDL_SCANCODE_ESCAPE]) {
		gEventQuit = true;
	}


	// All time units are in milliseconds
	static uint32_t sPrevTime = 0;
	uint32_t curTime = SDL_GetTicks();
	float deltaTime = static_cast<float>(curTime - sPrevTime);
	sPrevTime = curTime;

	if (gKeyPressed[SDL_SCANCODE_W]) {
		gCamera.Move(AXIS_Z, true, deltaTime);
	}
	if (gKeyPressed[SDL_SCANCODE_S]) {
		gCamera.Move(AXIS_Z, false, deltaTime);
	}
	if (gKeyPressed[SDL_SCANCODE_A]) {
		gCamera.Move(AXIS_X, false, deltaTime);
	}
	if (gKeyPressed[SDL_SCANCODE_D]) {
		gCamera.Move(AXIS_X, true, deltaTime);
	}
}
