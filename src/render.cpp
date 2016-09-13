
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "main.h"
#include "render.h"
#include "shader.h"

static Shader gCurShader;
static GLuint gVAO = 0;
static GLuint gTexture[2] = { 0 };


void Render_Setup()
{
	// Setup Viewport
	int width, height;	// Resolution (TODO: Get resolution from data)
	SDL_GetWindowSize(GetWindow() , &width, &height);
	glViewport(0, 0, width, height);

	SDL_GL_SetSwapInterval(1);	// Set vsync for now to avoid running hardware to max

	gCurShader.Load("basic", "basic");

	// Setup basic rectangle
	GLfloat vertices[] = {
		// Position				// Color			// Texture UVs
		0.5f, 0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,	// Top Right
		0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	1.0f, 0.0f,	// Bottom Right
		-0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f,	// Bottom Left
		-0.5f,  0.5f, 0.0f,		1.0f, 1.0f, 0.0f,	0.0f, 1.0f,	// Top Left
	};
	GLuint indices[] = {
		0, 1, 3,	// Triangle 1
		1, 2, 3,	// Triangle 2
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

	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Explanation:
	// - 1st parameter is which the index to send the data to. In basic.vert shader this is (location = 0)
	// - 2nd parameter is the size of the vertex attribute. It's a vec3 (for position) so it has 3 values.
	// - 3rd parameter is the type of the data, which are floats.
	// - 4th parameter sets whether the data should be normalized.
	// - 5th parameter is stride between each vertex's data. Data has position, color + uvs, which are 8 floats.
	// - Last parameter is offset where this data begins.
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
	unsigned char* textureData = stbi_load("../data/textures/brick.jpg", &texWidth, &texHeight, &bytesPerPixel, 3);
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
	textureData = stbi_load("../data/textures/face.jpg", &texWidth, &texHeight, &bytesPerPixel, 3);
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
	glClear(GL_COLOR_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gTexture[0]);
	glUniform1i( glGetUniformLocation(gCurShader.GetProgramID(), "mTexture1"), 0 );

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gTexture[1]);
	glUniform1i(glGetUniformLocation(gCurShader.GetProgramID(), "mTexture2"), 1);

	glUseProgram(gCurShader.GetProgramID());
	glBindVertexArray(gVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// Swap buffers
	SDL_GL_SwapWindow(GetWindow());
}
