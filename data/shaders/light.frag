#version 330 core
// vertex color + light

out vec4 color;

uniform vec3 objColor;
uniform vec3 lightColor;

void main()
{
	color = vec4(objColor * lightColor, 1.0);
}
