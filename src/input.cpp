#include "SDL/SDL.h"

#include "camera.h"


// ----------------------------------------------------
// Definitions
// ----------------------------------------------------

#define INPUT_MAX_MOUSE_BUTTONS	5


// ----------------------------------------------------
// Global Variables
// ----------------------------------------------------

extern Camera gCamera;


// ----------------------------------------------------
// Local Variables
// ----------------------------------------------------

static bool gEventQuit = false;
static bool gKeyPressed[SDL_NUM_SCANCODES] = { 0 };
static bool gMousePressed[INPUT_MAX_MOUSE_BUTTONS] = { 0 };
static int32_t gMouseMotionRel[2] = { 0 };	// Relative mouse motion
static int32_t gMouseWheelMotion = 0;

// Note: Scancodes are the physical location of the key on the keyboard, while
//       keycodes are soft-keys mapped to the location and are dependent on locale.
//       Keycodes should be used for character-dependent situations.


// ----------------------------------------------------

bool Input_GetQuitStatus()						{ return gEventQuit; }
bool Input_GetKeyPressStatus(int scancode)		{ return gKeyPressed[scancode]; }
bool Input_GetMousePressStatus(int buttonIndex)	{ return gMousePressed[buttonIndex]; }
int32_t Input_GetMouseMotionRelative()			{ return gMouseWheelMotion; }

void Input_GetMouseMotionRelative(int32_t* x, int32_t* y)
{
	if (x != nullptr) {
		*x = gMouseMotionRel[0];
	}
	if (y != nullptr) {
		*y = gMouseMotionRel[1];
	}
}

void Input_Process()
{
	// Need to reset relative motion every frame
	memset(gMouseMotionRel, 0, sizeof(gMouseMotionRel));
	gMouseWheelMotion = 0;

	SDL_Event event;
	while (SDL_PollEvent(&event) != 0) {
		switch (event.type)
		{
		case SDL_QUIT:
			gEventQuit = true;
			break;

		case SDL_KEYDOWN:
			gKeyPressed[event.key.keysym.scancode] = true;
			break;

		case SDL_KEYUP:
			gKeyPressed[event.key.keysym.scancode] = false;
			break;

		case SDL_MOUSEBUTTONDOWN:
			gMousePressed[event.button.button - 1] = true;
			break;

		case SDL_MOUSEBUTTONUP:
			gMousePressed[event.button.button - 1] = false;
			break;

		case SDL_MOUSEMOTION:
			gMouseMotionRel[0] = event.motion.xrel;
			gMouseMotionRel[1] = event.motion.yrel;
			break;

		case SDL_MOUSEWHEEL:
			gMouseWheelMotion = event.wheel.y;
			break;
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

	gCamera.ProcessInputs(deltaTime);
}
