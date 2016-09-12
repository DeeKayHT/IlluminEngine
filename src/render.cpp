
#include <stdio.h>

#include "main.h"
#include "render.h"
#include "shader.h"

static Shader gCurShader;
static GLuint gVAO = 0;


void RenderSetup()
{
	// Setup Viewport
	int width, height;	// Resolution (TODO: Get resolution from data)
	SDL_GetWindowSize(GetWindow() , &width, &height);
	glViewport(0, 0, width, height);

	SDL_GL_SetSwapInterval(1);	// Set vsync for now to avoid running hardware to max

	gCurShader.Load("basic", "basic");

	// Setup basic rectangle
	GLfloat vertices[] = {
		// Position				// Color
		0.5f, 0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	// Top Right
		0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	// Bottom Right
		-0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	// Bottom Left
		-0.5f,  0.5f, 0.0f,		1.0f, 1.0f, 0.0f,	// Top Left
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
	// - 5th parameter is stride between each vertex's data. Data has position + color, which are 6 floats.
	// - Last parameter is offset where this data begins.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Unbind the VBO and VAO to prevent any subsequent OpenGL calls from potentially modifying it
	glBindBuffer(GL_ARRAY_BUFFER, 0);	// Okay to unbind VBO since vertexAttribute registered the VBO
	glBindVertexArray(0);
}


void Render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(gCurShader.GetProgramID());
	glBindVertexArray(gVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// Swap buffers
	SDL_GL_SwapWindow(GetWindow());
}
