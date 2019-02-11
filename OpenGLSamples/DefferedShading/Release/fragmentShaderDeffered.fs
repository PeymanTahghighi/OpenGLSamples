#version 330

struct PointLight
{
	vec3 lightPosition;
	float linear;
	float quadratic;
	vec3 lightColor;
};

uniform sampler2D textureWorldPosition;
uniform sampler2D textureNormal;
uniform sampler2D textureAlbedoSpec;

uniform PointLight pointLight;
uniform vec3 cameraPosition;

in vec2 texCoord;

out vec4 fragColor;

void main()
{	
	vec3 positonWorld = texture(textureWorldPosition,texCoord).rgb;
	vec3 normalWorld = texture(textureNormal,texCoord).rgb;
	vec4 albedoSpec = texture(textureAlbedoSpec,texCoord);
	vec3 viewVec = normalize(cameraPosition - positonWorld);

	vec3 lightVec = normalize(pointLight.lightPosition - positonWorld);
	vec3 diffuse = max(dot(lightVec,normalWorld),0.0f) * pointLight.lightColor * albedoSpec.rgb;
	vec3 halfWay = normalize(lightVec + viewVec);
	vec3 specular = pow(max(dot(halfWay,normalWorld),0.0f),256.0f) * pointLight.lightColor * albedoSpec.rgb;
	vec3 finalColor = diffuse + specular;

	fragColor = vec4(finalColor,1.0f);
}