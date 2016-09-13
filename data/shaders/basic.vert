#version 330 core
  
layout (location = 0) in vec3 position;	// The position variable has attribute position 0
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 uv;

out vec3 vertColor;
out vec2 texCoord;

void main()
{
	vertColor = color;
	texCoord = uv;
    gl_Position = vec4(position.xyz, 1.0);
}
