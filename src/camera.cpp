#include <algorithm>
#include <stdio.h>

#include "glm/gtc/matrix_transform.hpp"
#include "SDL/SDL_scancode.h"

#include "camera.h"
#include "input.h"


Camera::Camera()
{
	mPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	mDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	mUp = glm::vec3(0.0f, 1.0f, 0.0f);
	mSpeed = 0.01f;
	mFOV = 45.0f;

	mView = glm::lookAt(mPosition, mPosition + mDirection, mUp);
}

Camera::~Camera()
{
}

void Camera::SetPosition(const glm::vec3 position)
{
	mPosition = position;
}

glm::mat4* Camera::GetViewMatrix()
{
	mView = glm::lookAt(mPosition, mPosition + mDirection, mUp);
	return &mView;
}

float Camera::GetFoV()
{
	return mFOV;
}

void Camera::Move(Axis_e axis, bool isPositive, float deltaTime)
{
	float multFactor = isPositive ? 1.0f : -1.0f;
	float distance = mSpeed * deltaTime * multFactor;

	switch (axis)
	{
		case AXIS_X:
			mPosition += glm::normalize(glm::cross(mDirection, mUp)) * distance;
			break;

		case AXIS_Z:
			mPosition += mDirection * distance;
			break;
	}
}

void Camera::Rotate()
{
	int32_t xoffset, yoffset;
	Input_GetMouseMotionRelative(&xoffset, &yoffset);

	static float s_Pitch = 0.0f;
	static float s_Yaw = -90.0f;
	float sensitivity = 0.075f;
	s_Pitch += static_cast<float>(yoffset) * sensitivity;
	s_Yaw += static_cast<float>(xoffset) * sensitivity;

	float yawRadians = glm::radians(s_Yaw);
	float pitchRadians = glm::radians(s_Pitch);
	mDirection.x = cosf(pitchRadians) * cosf(yawRadians);
	mDirection.y = sinf(pitchRadians);
	mDirection.z = cosf(pitchRadians) * sinf(yawRadians);
	mDirection = glm::normalize(mDirection);
}

void Camera::Zoom(int32_t zoom)
{
	float sensitivity = -3.0f;
	mFOV += static_cast<float>(zoom) * sensitivity;

	// Clamp the FOV!
	mFOV = std::max(mFOV, 1.0f);
	mFOV = std::min(mFOV, 90.0f);
}

void Camera::ProcessInputs(float deltaTime)
{
	if (Input_GetKeyPressStatus(SDL_SCANCODE_W)) {
		Move(AXIS_Z, true, deltaTime);
	}
	if (Input_GetKeyPressStatus(SDL_SCANCODE_S)) {
		Move(AXIS_Z, false, deltaTime);
	}
	if (Input_GetKeyPressStatus(SDL_SCANCODE_A)) {
		Move(AXIS_X, false, deltaTime);
	}
	if (Input_GetKeyPressStatus(SDL_SCANCODE_D)) {
		Move(AXIS_X, true, deltaTime);
	}
	if (Input_GetMousePressStatus(0)) {
		Rotate();
	}
	int32_t zoomVal = Input_GetMouseMotionRelative();
	if (zoomVal != 0) {
		Zoom(zoomVal);
	}
}
