#pragma once
#include "glm/glm.hpp"

typedef enum
{
	AXIS_X,
	AXIS_Y,
	AXIS_Z,
} Axis_e;

class Camera
{
private:
	glm::vec3 mDirection;
	glm::vec3 mUp;
	glm::mat4 mView;

	float mSpeed;
	float mFOV;		// Field of View (units in degrees)

public:
	glm::vec3 mPosition;

	Camera();
	~Camera();

	void SetPosition(const glm::vec3 position);
	glm::mat4*	GetViewMatrix();
	float		GetFoV();

	void Move(Axis_e axis, bool isPositive, float deltaTime);
	void Rotate();
	void Zoom(int32_t zoom);
	void ProcessInputs(float deltaTime);
};
