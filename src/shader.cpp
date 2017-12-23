#include <stdio.h>

#include "SDL/SDL.h"
#include "glm/gtc/type_ptr.hpp"

#include "shader.h"
#include "main.h"

#define COUNT(array) sizeof(array)/sizeof(array[0])


GLuint Shader::LoadFile(const char* filename, GLenum shaderType)
{
	if (filename == nullptr) {
		return 0;
	}

	// Determine file extention based on shader type
	char shaderExt[8];
	switch (shaderType)
	{
	case GL_VERTEX_SHADER:
		snprintf(shaderExt, COUNT(shaderExt), ".vert");
		break;

	case GL_FRAGMENT_SHADER:
		snprintf(shaderExt, COUNT(shaderExt), ".frag");
		break;

	default:
		printf("Cannot load unsupported shader type (0x%x)!\n", shaderType);
		return 0;
	}


	char path[512];
	snprintf(path, sizeof(path), "../../data/shaders/%s%s", filename, shaderExt);


	SDL_RWops* file = SDL_RWFromFile(path, "r");
	if (file == NULL) {
		printf("Failed to open shader: %s\n", filename);
		return 0;
	}

	GLint fileSize = static_cast<GLint>(SDL_RWsize(file));
	void* shaderBuffer = malloc(fileSize);
	if (shaderBuffer == nullptr) {
		printf("No memory to load shader: %s\n", filename);
		return 0;
	}

	size_t objsRead = SDL_RWread(file, shaderBuffer, fileSize, 1);
	SDL_RWclose(file);
	if (objsRead == 0) {
		printf("Failed to read shader: %s\n", filename);
		return 0;
	}


	// Load and compile the shader
	GLuint shaderID = glCreateShader(shaderType);
	glShaderSource(shaderID, 1, (GLchar**)&shaderBuffer, &fileSize);
	glCompileShader(shaderID);
	
	GLint success;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar infoLog[512];
		glGetShaderInfoLog(shaderID, sizeof(infoLog), NULL, infoLog);
		printf("%s\n", infoLog);

		glDeleteShader(shaderID);
		shaderID = 0;
	}

	free(shaderBuffer);
	return shaderID;
}


Shader::Shader()
{
	mProgramID = 0;
}

Shader::Shader(const char* vertexShaderName, const char* fragmentShaderName)
{
	mProgramID = 0;
	Load(vertexShaderName, fragmentShaderName);
}

Shader::~Shader()
{
	glDeleteProgram(mProgramID);
}

void Shader::Load(const char* vertexShaderName, const char* fragmentShaderName)
{
	if (vertexShaderName == nullptr || fragmentShaderName == nullptr) {
		return;
	}

	GLuint vertexShader = LoadFile(vertexShaderName, GL_VERTEX_SHADER);
	GLuint fragShader = LoadFile(fragmentShaderName, GL_FRAGMENT_SHADER);
	if (vertexShader == 0 || fragShader == 0) {
		glDeleteShader(vertexShader);
		glDeleteShader(fragShader);
		return;
	}


	// Attach the shaders to a program to be used
	mProgramID = glCreateProgram();
	glAttachShader(mProgramID, vertexShader);
	glAttachShader(mProgramID, fragShader);
	glLinkProgram(mProgramID);

	GLint success;
	glGetProgramiv(mProgramID, GL_LINK_STATUS, &success);
	if (!success) {
		GLchar infoLog[512];
		glGetProgramInfoLog(mProgramID, sizeof(infoLog), NULL, infoLog);
		printf("%s\n", infoLog);
	}

	// The shaders are no longer needed once linked to a program
	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);
}

void Shader::SetInt(const char* name, const int32_t value)
{
	GLint location = glGetUniformLocation(mProgramID, name);
	glUniform1i(location, value);
}

void Shader::SetFloat(const char* name, const float value)
{
	GLint location = glGetUniformLocation(mProgramID, name);
	glUniform1f(location, value);
}

void Shader::SetVec3(const char* name, const glm::vec3& value)
{
	GLint location = glGetUniformLocation(mProgramID, name);
	glUniform3fv(location, 1, glm::value_ptr(value));
}

void Shader::SetVec3(const char* name, const float x, const float y, const float z)
{
	GLint location = glGetUniformLocation(mProgramID, name);
	glUniform3f(location, x, y, z);
}

void Shader::SetMat4(const char* name, const glm::mat4& value)
{
	GLint location = glGetUniformLocation(mProgramID, name);
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

uint32_t Shader::GetProgramID() { return mProgramID; }
