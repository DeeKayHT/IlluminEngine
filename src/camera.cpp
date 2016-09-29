#include "camera.h"
#include "glm/gtc/matrix_transform.hpp"

Camera::Camera()
{
	mPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	mDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	mUp = glm::vec3(0.0f, 1.0f, 0.0f);
	mSpeed = 0.01f;

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

		default:
			break;
	}
}
