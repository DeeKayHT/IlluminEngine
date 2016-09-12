#version 330 core
  
layout (location = 0) in vec3 position;	// The position variable has attribute position 0
layout (location = 1) in vec3 color;

out vec3 vertColor;

void main()
{
	vertColor = color;
    gl_Position = vec4(position.xyz, 1.0);
}
