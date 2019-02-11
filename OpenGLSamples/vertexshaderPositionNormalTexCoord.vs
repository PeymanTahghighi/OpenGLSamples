#version 330

layout (location = 0) in vec3 Position0;
layout (location = 1) in vec3 Normal0;
layout (location = 2) in vec2 texCoord0;

out VS_OUT
{
	 vec2 texCoord;
	 vec3 normal;
	 vec3 fragPos;
	 vec4 fragPosLightSpace;
} vs_out;

uniform vec3 lightPos;

uniform mat4 model;
uniform mat4 lightMatrix;

layout (std140) uniform Matrices
{
	 mat4 projection;
	 mat4 view;
};



void main()
{
	gl_Position = projection * view * model * vec4(Position0 , 1.0f);
	vs_out.fragPos = vec3(model * vec4(Position0,1.0f));
	vs_out.texCoord = texCoord0.xy;
	vs_out.normal =  mat3(transpose(inverse(model))) * Normal0;
	vs_out.fragPosLightSpace = lightMatrix * vec4(vs_out.fragPos,1.0f);
}