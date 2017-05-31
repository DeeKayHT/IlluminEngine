#pragma once
#include "glm/glm.hpp"

class Light
{
public:
	union {
		glm::vec3	mPosition;
		glm::vec3	mDirection;
	};
	glm::vec3	mColor;
	float		mAmbientIntensity;

	Light(glm::vec3 position, glm::vec3 color);
};
