#version 330

layout (location = 0) in vec3 Position0;
layout (location = 1) in vec3 Normal0;
layout (location = 2) in vec2 texCoord0;

uniform mat4 model;
uniform mat4 lightSpaceMatrix;

void main()
{
	gl_Position = lightSpaceMatrix * model *  vec4(Position0 , 1.0f);
}