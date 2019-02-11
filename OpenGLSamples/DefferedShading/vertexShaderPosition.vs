#version 330

layout (location = 0) in vec3 Position0;
layout (location = 1) in vec2 texCoord0;

out vec2 texCoord;
out vec2 cpPos;

void main()
{
	
	gl_Position = vec4(Position0,1.0f);
	cpPos = gl_Position.xy;
	texCoord = texCoord0;
	
}