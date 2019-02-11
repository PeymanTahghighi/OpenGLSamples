#version 330
layout (location = 0) out vec3 normal;
layout (location = 1) out vec4 albedoSpec;
layout (location = 2) out vec3 specular;

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

uniform sampler2D diffuseTexture0; //diffuse texture0
uniform sampler2D specularTexture0; //specular texture0 


void main()
{
	normal = normalWorld * 0.5f + 0.5f;
	vec3 textureDiffuse = texture(diffuseTexture0,texCoord).rgb;
	vec3 textureSpecularMap = texture(specularTexture0,texCoord).rgb;
	albedoSpec = vec4(material.diffuse,material.shininess);
	specular = vec3(1.0f);
}
