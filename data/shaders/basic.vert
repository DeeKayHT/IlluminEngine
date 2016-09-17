#version 330 core
  
layout (location = 0) in vec3 position;	// The position variable has attribute position 0
layout (location = 1) in vec2 uv;

out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	texCoord = uv;
    gl_Position = projection * view * model * vec4(position.xyz, 1.0);
}
