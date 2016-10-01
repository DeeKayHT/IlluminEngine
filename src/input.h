#pragma once

bool Input_GetQuitStatus();
bool Input_GetKeyPressStatus(int scancode);
bool Input_GetMousePressStatus(int buttonIndex);
int32_t Input_GetMouseMotionRelative();
void Input_GetMouseMotionRelative(int32_t* x, int32_t* y);
void Input_Process();