
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "main.h"
#include "light.h"
#include "render.h"
#include "shader.h"
#include "camera.h"


// ----------------------------------------------------
// Global Variables
// ----------------------------------------------------

Camera gCamera;


// ----------------------------------------------------
// Local Variables
// ----------------------------------------------------

static Shader gCurShader;
static Shader gLightShader;
static GLuint gVAO = 0;
static GLuint glightVAO = 0;
static GLuint gTexture[2] = { 0 };

static int gWidth = 0;
static int gHeight = 0;

static glm::vec3 gCubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};


// ----------------------------------------------------

void Render_Setup()
{
	// Setup Viewport
	SDL_GetWindowSize(GetWindow() , &gWidth, &gHeight);
	glViewport(0, 0, gWidth, gHeight);

	SDL_GL_SetSwapInterval(1);	// Set vsync for now to avoid running hardware to max

	gCurShader.Load("basic", "basic");
	gLightShader.Load("light_point", "light_point");

	glEnable(GL_DEPTH_TEST);

	// Setup cube
	GLfloat vertices[] = {
		// Position           Normals				Texture Coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,	0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,	1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,	1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,	1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,	0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,	0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,	0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,	1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,	1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,	1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,	0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,	0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,	1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,	1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,	0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,	0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,	0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,	1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,	1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,	1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,	0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,	0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,	0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,	1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,	0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,	1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,	1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,	1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,	0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,	0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,	0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,	1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,	1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,	1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,	0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,	0.0f,  1.0f
	};


	// Create a VAO to store vertex attribute data with VBOs
	glGenVertexArrays(1, &gVAO);
	glBindVertexArray(gVAO);	// Subsequent OpenGL calls with VBOs and vertex attributes will affect this VAO

	// Create a vertex buffer object (VBO) to send
	// vertex data to the GPU as a single data stream
	GLuint VBO;
	glGenBuffers(1, &VBO);				// Create one VBO and store it in variable VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);	// Bind VBO to the GL_ARRAY_BUFFER target
										// Now send the vertex data to the VBO target
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	// Explanation:
	// - 1st parameter is which the index to send the data to. In basic.vert shader this is (location = 0)
	// - 2nd parameter is the size of the vertex attribute. It's a vec3 (for position) so it has 3 values.
	// - 3rd parameter is the type of the data, which are floats.
	// - 4th parameter sets whether the data should be normalized.
	// - 5th parameter is stride between each vertex's data. Data has position + normals + uvs, which are 8 floats.
	// - Last parameter is offset where this data begins.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);


	// Create a new VAO for the light cube
	glGenVertexArrays(1, &glightVAO);
	glBindVertexArray(glightVAO);	// Subsequent OpenGL calls with VBOs and vertex attributes will affect this VAO

	// Bind VBO to the GL_ARRAY_BUFFER target
	// Can reuse the same VBO as above since it uses the same cube vertices
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// Unbind the VBO and VAO to prevent any subsequent OpenGL calls from potentially modifying it
	glBindBuffer(GL_ARRAY_BUFFER, 0);	// Okay to unbind VBO since vertexAttribute registered the VBO
	glBindVertexArray(0);


	// -------------------------------------------------------------
	// Load texture1
	// Enforce the image loaded to have 3-components per pixel (RGB)
	// TODO: Flip the image before sending to OpenGL
	int texWidth, texHeight, bytesPerPixel;
	unsigned char* textureData = stbi_load("../data/textures/container2.png", &texWidth, &texHeight, &bytesPerPixel, 3);
	if (textureData == NULL)
		return;

	// Create and bind the texture (subsequent calls to GL_TEXTURE_2D will apply to the bound texture)
	glGenTextures(1, &gTexture[0]);
	glBindTexture(GL_TEXTURE_2D, gTexture[0]);
	// Set textures filtering (options: nearest, linear)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// mipmaps only apply when textures get downscaled
	// Set texture wrapping behavior (options: repeat, mirrored repeat, clamp to edge, clamp to border)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// Now load the texture!
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Texture cleanup
	stbi_image_free(textureData);		// Texture loaded into OpenGL, no longer needed in memory!


	// -------------------------------------------------------------
	// Load texture2
	textureData = stbi_load("../data/textures/container2_specular.png", &texWidth, &texHeight, &bytesPerPixel, 3);
	if (textureData == NULL)
		return;

	// Create and bind the texture (subsequent calls to GL_TEXTURE_2D will apply to the bound texture)
	glGenTextures(1, &gTexture[1]);
	glBindTexture(GL_TEXTURE_2D, gTexture[1]);
	// Set textures filtering (options: nearest, linear)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// mipmaps only apply when textures get downscaled
																		// Set texture wrapping behavior (options: repeat, mirrored repeat, clamp to edge, clamp to border)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// Now load the texture!
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Texture cleanup
	stbi_image_free(textureData);		// Texture loaded into OpenGL, no longer needed in memory!
	glBindTexture(GL_TEXTURE_2D, 0);	// Unbind the texture so future texture calls don't modify it!


	// Set the initial camera position
	gCamera.SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
}

void Render_Shutdown()
{
	glDeleteVertexArrays(1, &gVAO);
	// TODO: When those variables aren't local to Render_Setup(). D'oh.
	//glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);
}


void Render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gTexture[0]);
	glUniform1i(glGetUniformLocation(gCurShader.GetProgramID(), "mTexture1"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gTexture[1]);
	glUniform1i(glGetUniformLocation(gCurShader.GetProgramID(), "mTexture2"), 1);

	
	// Camera Data
	gLightShader.SetVec3("viewPos", gCamera.mPosition);

	// Material Data
	gLightShader.SetInt("material.diffuse", 0);
	gLightShader.SetInt("material.specular", 1);
	gLightShader.SetFloat("material.shininess", 32.0f);

	// Light data
	Light pointLight(glm::vec3(1.2f, 1.0f, 2.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	gLightShader.SetVec3("light.position", pointLight.mPosition);
	gLightShader.SetVec3("light.ambient", 0.2f, 0.2f, 0.2f);
	gLightShader.SetVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
	gLightShader.SetVec3("light.specular", 1.0f, 1.0f, 1.0f);
	gLightShader.SetFloat("light.constant", 1.0f);
	gLightShader.SetFloat("light.linear", 0.09f);
	gLightShader.SetFloat("light.quadratic", 0.032f);


	// Modify transformation per frame
	float seconds = static_cast<float>(SDL_GetTicks()) / 1000.0f;



	// Setup the camera's view matrix
	glm::mat4* view = gCamera.GetViewMatrix();
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(gCamera.GetFoV()), (GLfloat)gWidth / (GLfloat)gHeight, 0.1f, 100.0f);

	gLightShader.SetMat4("view", *view);
	gLightShader.SetMat4("projection", projection);

	glUseProgram(gLightShader.GetProgramID());
	// Draw the cubes
	glBindVertexArray(gVAO);
	for (uint32_t i = 0; i < 10; i++) {
		float angle = 20.0f * i;

		glm::mat4 model;	// Default creates identity matrix
		model = glm::translate(model, gCubePositions[i]);
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		gLightShader.SetMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glBindVertexArray(0);


	// Swap buffers
	SDL_GL_SwapWindow(GetWindow());
}
