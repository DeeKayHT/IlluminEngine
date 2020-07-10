#pragma once
#include "glm/glm.hpp"

class Light
{
public:
	union {
		glm::vec3	mPosition;
		glm::vec3	mDirection;
	};
	glm::vec3	mAmbient;
	glm::vec3	mDiffuse;
	glm::vec3	mSpecular;

	Light(glm::vec3 position);

	void setAmbient(float r, float g, float b);
	void setDiffuse(float r, float g, float b);
	void setSpecular(float r, float g, float b);
};
