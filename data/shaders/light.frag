#version 330 core
// vertex color + light

in vec3 fPos;
in vec3 fNormal;

out vec4 color;

uniform vec3 objColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	vec3 normal = normalize(fNormal);
	vec3 lightDir = normalize(lightPos - fPos);
	
	float ambientIntensity = 0.1f;
	vec3 ambient = ambientIntensity * lightColor;
	
	float diffuseIntensity = max( dot(normal, lightDir), 0.0f );
	vec3 diffuse = diffuseIntensity * lightColor;
	
	vec3 viewDir = normalize(viewPos - fPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	
	float specIntensity = 0.5f;
	float spec = pow( max( dot(viewDir, reflectDir), 0.0f), 32.0f);
	vec3 specular = specIntensity * spec * lightColor;
	
	color = vec4(objColor * (ambient + diffuse + specular), 1.0);
}
