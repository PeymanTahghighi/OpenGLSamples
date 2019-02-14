#version 330

struct PointLight
{
	vec3 lightPosition;
	float linear;
	float quadratic;
	vec3 lightColor;
};

struct DirectionalLight
{
	vec3 direction;
	vec3 color;
};

uniform sampler2D textureDepth;
uniform sampler2D textureNormal;
uniform sampler2D textureAlbedoSpec;
uniform sampler2D textureSpecular;
uniform sampler2D directionalLightShadowmap;

uniform DirectionalLight directionalLight;
uniform vec3 cameraPosition;
uniform float projA;
uniform float projB;
uniform float projParamX;
uniform float projParamY;
uniform mat4 invView;
uniform mat4 lightSpaceMatrix;

in vec2 texCoord;
in vec2 cpPos;



out vec4 fragColor;


float calculateShadow(vec4 lightSpacePosition,vec3 normal)
{
	vec3 projCoords = lightSpacePosition.xyz / lightSpacePosition.w;
	projCoords.xyz = projCoords.xyz * vec3(0.5f) + vec3(0.5f);
	float closestDepth = texture(directionalLightShadowmap,projCoords.xy).r;
	float actualDepth = projCoords.z;
	float slopeScaleBias = max(0.05f * 1.0f - max(dot(-directionalLight.direction,normal),0.0f),0.005f);
	float texelSize = 1.0f/1024.0f;
	float average = 0.0f;
	float samples = 3.0f;
	for(float i = ceil(-samples/2.0f); i < floor(samples / 2.0f) ; ++i)
	{
		for(float j = ceil(-samples/2.0f); j < floor(samples / 2.0f); j++)
		{
			float sampledDepth = texture(directionalLightShadowmap,projCoords.xy + vec2(i*texelSize,j*texelSize)).r;
			if(actualDepth - slopeScaleBias < sampledDepth)
				average+=1.0f;
		}
	}
	return average / (samples*samples);
}

void main()
{	
	//Sample textures
	vec3 normalWorld = texture(textureNormal,texCoord).rgb;
	vec4 albedoSpec = texture(textureAlbedoSpec,texCoord);
	vec4 specularMap = texture(textureSpecular,texCoord);
	float depth = texture(textureDepth,texCoord).r;
	//-------------------------------------------------------------------

	//reconstruct world position from depth
	float zClip = depth * 2.0f - 1.0f;
	float zEye = projB / (zClip + projA);
	vec3 viewPos = vec3(cpPos.x * projParamX * zEye,cpPos.y * projParamY * zEye,-zEye);
	vec3 positonWorld = ((invView) * vec4(viewPos,1.0f)).xyz;
	//--------------------------------------------------------------------

	//calculate necessary vectors
	vec3 viewVec = normalize(cameraPosition - positonWorld);
	normalWorld = normalWorld * 2.0f - 1.0f;
	vec4 lightSpacePosition = lightSpaceMatrix * vec4(positonWorld,1.0f);
	//-------------------------------------------------------------------

	float shadow = calculateShadow(lightSpacePosition,normalWorld);
	//calculate lighting
	vec3 diffuse = max(dot(-directionalLight.direction,normalWorld),0.0f) * directionalLight.color * albedoSpec.rgb;
	vec3 halfWay = normalize(-directionalLight.direction + viewVec);
	vec3 specular = pow(max(dot(halfWay,normalWorld),0.0f),128.0f) * directionalLight.color * albedoSpec.rgb * specularMap.rgb;
	diffuse*=shadow;
	specular*=shadow;
	vec3 finalColor = diffuse + specular;
	//-------------------------------------------------------------------
	
	fragColor = vec4(finalColor,1.0f);
}