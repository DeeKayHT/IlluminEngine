#pragma once

#include "GL/glew.h"

int32_t Render_GetVSync();
void Render_SetVSync(int8_t mode);

void Render_Setup();
void Render_Shutdown();
void Render();
