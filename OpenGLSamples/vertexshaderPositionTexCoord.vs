#version 330

layout (location = 0) in vec3 Position0;
layout(location = 1) in vec2 texCoord0;


uniform mat4 model;

layout (std140) uniform Matrices
{
	mat4 projection;
	mat4 view;
};

out vec2 texCoord;

void main()
{
	gl_Position = projection * view * model * vec4(Position0 , 1.0f);
	texCoord = texCoord0;
}