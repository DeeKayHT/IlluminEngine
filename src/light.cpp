#include "light.h"

Light::Light(glm::vec3 position)
{
	mPosition = position;
}

void Light::setAmbient(float r, float g, float b)
{
	mAmbient.r = r;
	mAmbient.g = g;
	mAmbient.b = b;
}

void Light::setDiffuse(float r, float g, float b)
{
	mDiffuse.r = r;
	mDiffuse.g = g;
	mDiffuse.b = b;
}

void Light::setSpecular(float r, float g, float b)
{
	mSpecular.r = r;
	mSpecular.g = g;
	mSpecular.b = b;
}
