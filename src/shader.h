#pragma once
#include <stdint.h>
#include "GL/glew.h"

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

	uint32_t GetProgramID();
};
