#version 330 core
// vertex color + light

struct Material_t
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light_t
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 fPos;
in vec3 fNormal;

out vec4 color;

uniform vec3 objColor;
uniform vec3 viewPos;

uniform Material_t material;
uniform Light_t light;

void main()
{
	vec3 normal = normalize(fNormal);
	vec3 lightDir = normalize(light.position - fPos);
	
	vec3 ambient = light.ambient * material.ambient;
	
	float diffuseIntensity = max( dot(normal, lightDir), 0.0f );
	vec3 diffuse = material.diffuse * diffuseIntensity * light.diffuse;
	
	vec3 viewDir = normalize(viewPos - fPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	
	float spec = pow( max( dot(viewDir, reflectDir), 0.0f), 32.0f);
	vec3 specular = material.specular * spec * light.specular;
	
	color = vec4(ambient + diffuse + specular, 1.0f);
}
