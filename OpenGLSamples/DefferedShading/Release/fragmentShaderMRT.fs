#version 330
layout (location = 0) out vec3 position;
layout (location = 1) out vec3 normal;
layout (location = 2) out vec4 albedoSpec;

struct Material
{
	vec3 diffuse;
	vec3 specular;
	vec3 ambient;
	float shininess;
};

uniform Material material;

in vec3 normalWorld;
in vec3 worldPosition;
in vec2 texCoord;

void main()
{
	position = worldPosition;
	normal = normalWorld;
	albedoSpec = vec4(material.diffuse.r,material.diffuse.g,material.diffuse.b,material.shininess);
}
