#version 330

struct PointLight
{
	vec3 lightPosition;
	float linear;
	float quadratic;
	vec3 lightColor;
};

uniform sampler2D textureDepth;
uniform sampler2D textureNormal;
uniform sampler2D textureAlbedoSpec;
uniform sampler2D textureSpecular;

uniform PointLight pointLight[3];
uniform vec3 cameraPosition;
uniform float projA;
uniform float projB;
uniform float projParamX;
uniform float projParamY;
uniform mat4 invView;

in vec2 texCoord;
in vec2 cpPos;



out vec4 fragColor;


void main()
{	
	float depth = texture(textureDepth,texCoord).r;
	vec3 normalWorld = texture(textureNormal,texCoord).rgb;
	normalWorld = normalWorld * 2.0f - 1.0f;
	vec4 albedoSpec = texture(textureAlbedoSpec,texCoord);
	vec4 specularMap = texture(textureSpecular,texCoord);
	float zClip = depth * 2.0f - 1.0f;
	float zEye = projB / (zClip + projA);
	vec3 viewPos = vec3(cpPos.x * projParamX * zEye,cpPos.y * projParamY * zEye,-zEye);
	vec3 positonWorld = ((invView) * vec4(viewPos,1.0f)).xyz;
	vec3 viewVec = normalize(cameraPosition - positonWorld);

	vec3 diffuse = vec3(0.0f);
	vec3 specular = vec3(0.0f);
	for(int i =0; i < 3; i++)
	{
		vec3 lightVec = normalize(pointLight[i].lightPosition - positonWorld);
		diffuse += max(dot(lightVec,normalWorld),0.0f) * pointLight[i].lightColor * albedoSpec.rgb;
		vec3 halfWay = normalize(lightVec + viewVec);
		specular += pow(max(dot(halfWay,normalWorld),0.0f),albedoSpec.a) * pointLight[i].lightColor * albedoSpec.rgb * specularMap.rgb;
	}
	
	vec3 finalColor = diffuse + specular;
	fragColor = vec4(finalColor.rgb,1.0f);
}