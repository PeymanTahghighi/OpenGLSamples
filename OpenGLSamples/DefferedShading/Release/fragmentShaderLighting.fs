#version 330

//------------------------------------------------------------------------
struct Material
{
	vec3 diffuse;
	vec3 specular;
	vec3 ambient;
	float shininess;
};

struct PointLight
{
	vec3 lightPosition;
	float linear;
	float quadratic;
	vec3 lightColor;
};

struct Spotlight
{
	vec3 lightPosition;
	vec3 lightDirection;
	vec3 lightColor;
	float lightFallOff;
	float lightFallOffOffset;
};
//-----------------------------------------------------------------------

out vec4 fragColor;

in vec3 worldPosition;
in vec3 normalWorld;
in vec2 texCoord;

uniform vec3 lightDirection;
uniform vec3 cameraPosition;
uniform float farPlane;

uniform Material material;
uniform PointLight pointLight;
uniform Spotlight spotLight;

uniform sampler2D texture0;
uniform samplerCube depthmapPointlight;
uniform samplerCube depthmapSpotlight;

float calculateShadow(samplerCube depthmap, vec3 fragPos,vec3 lightPos,vec3 lightVec)
{
	vec3 fragToLight = fragPos - lightPos;
	vec3 fragToLightDir = normalize(fragToLight);
	vec3 sampleVectors[10];

	float PoissonDisc1D[10] = float[](
	20.0f,
	10.0f,
	15.0f,
	12.0f,
	9.0f,
	5.0f,
	13.0f,
	11.0f,
	21.0f,
	22.0f
	);

	float depth = length(fragToLight);
	for(int i = 0; i < 10; ++i)
	{
		sampleVectors[i] = fragToLightDir;
		sampleVectors[i].z+=sin(radians(PoissonDisc1D[i]));
		sampleVectors[i] *=depth;
	}
	float biasFactor = 1.0f - clamp(dot(normalWorld,-lightVec),0.0,1.0f);
	float bias = 0.0f;

	vec3 PoissonDisc[20] = vec3[](
	vec3(0.656178f, 0.785943f, 0.0918607f),
	vec3(0.0980255f, 0.770562f, 0.888882f),
	vec3(0.353252f, 0.255623f, 0.0786767f),
	vec3(0.618091f, 0.510575f, 0.594409f),
	vec3(0.0362255f, 0.71392f, 0.369793f),
	vec3(0.948057f, 0.122684f, 0.21366f),
	vec3(0.986175f, 0.0407117f, 0.76928f),
	vec3(0.248604f, 0.0666524f, 0.984375f),
	vec3(0.972198f, 0.96704f, 0.662496f),
	vec3(0.635456f, 0.410657f, 0.909421f),
	vec3(0.894436f, 0.695456f, 0.351756f),
	vec3(0.990631f, 0.0898465f, 0.475967f),
	vec3(0.0231635f, 0.0941496f, 0.698111f),
	vec3(0.812677f, 0.327525f, 0.700522f),
	vec3(0.582232f, 0.955535f, 0.728751f),
	vec3(0.650258f, 0.0143742f, 0.585223f),
	vec3(0.0383618f, 0.523209f, 0.759117f),
	vec3(0.446425f, 0.650929f, 0.331828f),
	vec3(0.106143f, 0.437605f, 0.248207f),
	vec3(0.710746f, 0.0986663f, 0.133061f)
	);
	float visible = 1.0f;
	float radius = 1.0f;
	float average = 0.0f;
	int samples = 10;
	for(int i  = 0; i < 10; ++i)
	{
		float sampledDepth = texture(depthmap,sampleVectors[i]).r * farPlane;
		if(depth - bias > sampledDepth)
			visible -= 0.1f;
	}

	return clamp(visible,0.0f,1.0f);
}

bool equalZero(vec3 vector)
{
	if(vector.x < 0.001f && vector.y < 0.001f && vector.z < 0.01f)
		return true;
	return false;
}

float doSpotLight(vec3 lightDir,vec3 lightVec,float lightFallOff,float lightFallOffOffset)
{
	float theta = acos(dot(lightVec,normalize(lightDir)));
	float gamma = radians(lightFallOffOffset + lightFallOff);
	float intensity = clamp((theta / (radians(lightFallOff) - gamma)) - (gamma / (radians(lightFallOff) - gamma)),0.0f,1.0f);
	return intensity;
}

float doPointLight(vec3 lightVec)
{
	float distanceToLight = length(lightVec);
	return  1.0f / (1.0f + pointLight.linear * distanceToLight + pointLight.quadratic * distanceToLight  * distanceToLight);	
}

void main()
{
	fragColor = vec4(normalWorld,1.0f);
}
