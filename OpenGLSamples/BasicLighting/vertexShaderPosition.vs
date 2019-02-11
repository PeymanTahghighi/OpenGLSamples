#version 330

layout (location = 0) in vec3 Position0;
layout (location = 1) in vec3 Normal0;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;


void main()
{
	
	gl_Position = projection * view * model * vec4(Position0,1.0f);
	
}