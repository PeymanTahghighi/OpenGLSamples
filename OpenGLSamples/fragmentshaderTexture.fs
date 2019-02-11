#version 330

//structs
struct Material
{
	vec3 diffuse;
	vec3 specular;
	vec3 ambient;
	float shininess;
};

struct PointLight
{
	vec3 color;
	vec3 position;
	float constant;
	float linear;
	float quadratic;
};

struct SpotLight
{
	vec3 color;
	vec3 position;
	vec3 direction;
	float cutoff;
	float outterCutoff;
};

struct DirectionalLight
{
	vec3 direction;
	vec3 color;
};
//------------------------------------------------------------------------------------

in VS_OUT
{
	vec2 texCoord;
	vec3 normal;
	vec3 fragPos;
	vec4 fragPosLightSpace;
} fs_in;


layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

uniform sampler2D diffuseTexture0;//normal diffuse texture0
uniform sampler2D diffuseTexture1;//normal diffuse texture1
uniform sampler2D diffuseTexture2;//normal diffuse texture2


uniform sampler2D specularTexture0;//specular map texture0
uniform sampler2D specularTexture1;//specular map texture1
uniform sampler2D specularTexture2;//specular map texture2

uniform sampler2D ambientTexture0;//ambient map texture0
uniform sampler2D ambientTexture1;//ambient map texture1
uniform sampler2D ambientTexture2;//ambient map texture2

uniform samplerCube skybox;
uniform samplerCube shadowMap;

layout (std140) uniform Lights
{
	DirectionalLight directionalLight;
};

uniform vec3 cameraPosition;
uniform Material material;
uniform SpotLight spotLight;
uniform PointLight pointLight;

uniform float far_plane;
uniform int enableLighting = 1;

////-----------------------------------------------------------------------------------------------------------
//float calcShadowMapDirectional(vec3 normal,vec4 fragPosLightSpace)
//{
//	 vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
//    // transform to [0,1] range
//    projCoords = projCoords * 0.5 + 0.5;
//    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
//    float closestDepth = texture(shadowMap, projCoords.xy).r; 
//    // get depth of current fragment from light's perspective
//    float currentDepth = projCoords.z;
//    // check whether current frag pos is in shadow
//	vec3 lightDir = normalize(directionalLight.direction);
//    float bias = max(0.01f * (1.0f - dot(normal,lightDir)),0.001f);
//	//float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0; 
//
//	//pcf
//	vec2 texelSize = 1.0f / textureSize(shadowMap,0);
//	float shadow =0.0f;
//	for(int i=-1 ; i<=1 ; ++i)
//	{
//		for(int j=-1 ; j<=1 ; j++)
//		{
//			float pcfDepth = texture(shadowMap,projCoords.xy + vec2(i,j) * texelSize).r;
//			shadow += currentDepth -bias > pcfDepth ? 1.0f : 0.0f;
//		}
//	}
//	shadow/=9.0f;
//
//    return shadow;
//}
////-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------
float calcShadowMapOmniDirectional(vec3 fragPos)
{
	vec3 fragToLight = fragPos - pointLight.position;
	//float closestDepth = texture(shadowMap,fragToLight).r;
	float currentDepth = length(fragToLight);
	float viewDistance = length(cameraPosition - fragPos);
	////pcf
	//
	//float offset = 0.1f;
	//float samples = 4.0f;
	
	//float bias = 0.05f;
	//float shadow = 0.0f;
	//
	//for(float x = -offset; x < offset; x += offset / (samples * 0.5))
	//{
	//	for(float y = -offset; y < offset; y += offset / (samples * 0.5))
	//	{
	//		for(float z = -offset; z < offset; z += offset /(samples*0.5f))
	//		{
	//			float pcfDepth = texture(shadowMap,fragToLight + vec3(x,y,z)).r;
	//			pcfDepth*=far_plane;
	//			if(currentDepth - bias > pcfDepth)
	//				shadow += 1.0f;
	//		}
	//	}
	//}
	//shadow /=(samples * samples * samples);

	vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
	);

	float radius = (1.0f + (viewDistance / far_plane))/25.0f;
	float  samples = 20;
	float bias = 0.05f;
	float shadow = 0.0f;

	for(int i = 0; i < samples; ++i)
	{
		float depth = texture(shadowMap,fragToLight + sampleOffsetDirections[i] * radius).r;
		depth *= far_plane;
		if(currentDepth - bias > depth)
			shadow+=1.0f;
	}
	shadow /= samples;


	return shadow;
}
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------
vec4 calcDirectionalLight(vec3 normal,vec3 viewDir,vec4 diffuseTexture,vec4 specularTexture,vec4 ambientTexture,float shadow)
{
	//ambient lighting
	float ambientStrength = 1.2f;
	vec4 ambient = vec4(directionalLight.color * material.ambient * ambientTexture.rgb,1.0f) * ambientStrength;
	//-----------------------------------------------------

	//diffuse lighting
	float diffuseIntensity = max(dot(normalize(-directionalLight.direction),normal),0.0f);
	vec4 diffuse = vec4(diffuseIntensity * directionalLight.color * material.diffuse * diffuseTexture.rgb,1.0f);
	//-----------------------------------------------------

	//specular lighting
	vec3 halfWay = normalize(normalize(-directionalLight.direction)+viewDir);
	//vec3 reflect = reflect(normalize(-directionalLight.direction),normal);
	float specularIntensity = pow(max(dot(halfWay,normal),0.0f),material.shininess);
	vec4 specular = vec4(directionalLight.color * specularIntensity * material.specular * specularTexture.rgb,1.0f);
	//------------------------------------------------------

	return vec4(ambient.rgb+((1.0f - shadow)*(diffuse.rgb+specular.rgb)),1.0f);
}
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------
vec4 calcPointLight(PointLight pointLight, vec3 normal,vec3 viewDir,vec4 diffuseTexture,vec4 specularTexture,vec4 ambientTexture,float shadow)
{
	//ambient lighting
	float ambientStrength = 0.2f;
	vec4 ambient = vec4(pointLight.color * material.ambient * ambientTexture.rgb,1.0f) * ambientStrength;
	//-----------------------------------------------------

	//diffuse lighting
	vec3 lightDir = normalize(pointLight.position - fs_in.fragPos);
	float diffuseIntensity = max(dot(lightDir,normal),0.0f);
	vec4 diffuse = vec4(diffuseIntensity * pointLight.color * material.diffuse * diffuseTexture.rgb,1.0f);
	//-----------------------------------------------------

	//specular lighting
	vec3 halfWay = normalize(lightDir+viewDir);
	//vec3 reflect = reflect(normalize(-lightDir),normal);
	float specularIntensity = pow(max(dot(halfWay,normal),0.0f),material.shininess);
	vec4 specular = vec4(pointLight.color * specularIntensity * material.specular * specularTexture.rgb,1.0f);
	//------------------------------------------------------

	//point light
	float distance = length(pointLight.position - fs_in.fragPos);
	float attenuation = 1.0f / (pointLight.constant + (pointLight.linear * distance) + (pointLight.quadratic * pow(distance , 2)));

	specular *= attenuation;
	diffuse *= attenuation;
	ambient *= attenuation;
	//------------------------------------------------------

	return vec4(ambient.rgb+((1.0f - shadow) * (diffuse.rgb+specular.rgb)),1.0f);
}
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------
vec4 calcSpotLight(SpotLight spotLight,vec3 normal,vec3 viewDir,vec4 diffuseTexture,vec4 specularTexture,vec4 ambientTexture)
{
	//ambient lighting
	float ambientStrength = 0.2f;
	vec4 ambient = vec4(spotLight.color * material.ambient * ambientTexture.rgb,1.0f) * ambientStrength;
	//-----------------------------------------------------

	//diffuse lighting
	vec3 lightDir = normalize(spotLight.position - fs_in.fragPos);
	float diffuseIntensity = max(dot(lightDir,normal),0.0f);
	vec4 diffuse = vec4(diffuseIntensity * spotLight.color * material.diffuse * diffuseTexture.rgb,1.0f);
	//-----------------------------------------------------

	//specular lighting
	vec3 halfWay = normalize(viewDir+lightDir);
	//vec3 reflect = reflect(normalize(-lightDir),normal);
	float specularIntensity = pow(max(dot(halfWay,normal),0.0f),material.shininess);
	vec4 specular = vec4(spotLight.color * specularIntensity * material.specular * specularTexture.rgb,1.0f);
	//------------------------------------------------------

	//spot light
	float theta = max(dot(normalize(-lightDir),spotLight.direction),0.0f);
	float epsilon = spotLight.cutoff - spotLight.outterCutoff;
	float intensity = clamp((theta - spotLight.outterCutoff) / epsilon,0.0f,1.0f);

	if(theta < spotLight.cutoff)
	{
		ambient*=intensity;
		diffuse*=intensity;
		specular*=intensity;
	}
	//------------------------------------------------------


	return vec4(ambient.rgb+diffuse.rgb+specular.rgb,1.0f);
}
//-----------------------------------------------------------------------------------------------------------


void main()
{
	if(enableLighting == 0)
	{
		FragColor = vec4(2.0f,2.0f,2.0f,1.0f);
	}
	else
	{
		//calculate required values
		vec3 normalizedNormal = normalize(fs_in.normal);
		vec3 viewDir = normalize(cameraPosition - fs_in.fragPos);
		//------------------------------------------------------

		//calculate reflection
		float ratio = 1.00 / 2.42;
		vec3 I = fs_in.fragPos - cameraPosition;
		vec3 R = refract(I,normalizedNormal,ratio);
		vec4 reflectColor = vec4(texture(skybox,R).rgb,1.0f);
		//-------------------------------------------------------

		//extract textures
		vec4 diffuseTexture = texture(diffuseTexture0,fs_in.texCoord);
		vec4 specularTexture = texture(specularTexture0,fs_in.texCoord);
		vec4 ambientTexture = texture(ambientTexture0,fs_in.texCoord);
		
		//if no ambient texture is used
		if(ambientTexture == vec4(0.0f))
			ambientTexture = diffuseTexture;

		if(specularTexture == vec4(0.0f))
			specularTexture = vec4(1.0f);
		//------------------------------------------------------

		float shadow = calcShadowMapOmniDirectional(fs_in.fragPos);
		
		
		//calculate directional lighting first
		//vec4 finalColor = calcDirectionalLight(normalizedNormal,viewDir,diffuseTexture,specularTexture,ambientTexture,shadow);
		//------------------------------------------------------
		
		vec4 finalColor = calcPointLight(pointLight,normalizedNormal,viewDir,diffuseTexture,specularTexture,ambientTexture,shadow);
		//------------------------------------------------------
		
		//calculate spot light
		//finalColor += calcSpotLight(spotLight,normalizedNormal,viewDir,diffuseTexture,specularTexture,ambientTexture);
		//------------------------------------------------------
		
		
		FragColor = vec4(finalColor.rgb,1.0f);
	}
	 // check whether fragment output is higher than threshold, if so output as brightness color
    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(FragColor.rgb, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
	
}