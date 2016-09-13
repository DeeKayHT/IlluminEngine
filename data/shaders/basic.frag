#version 330 core

in vec3 vertColor;
in vec2 texCoord;

out vec4 color;

uniform sampler2D mTexture1;
uniform sampler2D mTexture2;

void main()
{
	color = mix(texture(mTexture1, texCoord), texture(mTexture2, texCoord), 0.2) * vec4(vertColor, 1.0);
}
