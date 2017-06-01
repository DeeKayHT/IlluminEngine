#pragma once
#include <stdint.h>

#include "GL/glew.h"
#include "glm/glm.hpp"

class Shader
{
private:
	uint32_t mProgramID;	// OpenGL-provided ID for a shader program (0 = invalid)

	GLuint LoadFile(const char* filename, GLenum shaderType);

public:
	Shader();
	Shader(const char* vertexShaderName, const char* fragmentShaderName);
	~Shader();

	void Load(const char* vertexShaderName, const char* fragmentShaderName);

	void SetInt(const char* name, const int32_t value);
	void SetFloat(const char* name, const float value);
	void SetVec3(const char* name, const glm::vec3& value);
	void SetVec3(const char* name, const float x, const float y, const float z);
	void SetMat4(const char* name, const glm::mat4& value);

	uint32_t GetProgramID();
};
