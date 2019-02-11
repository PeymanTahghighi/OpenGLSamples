#version 330

layout (location = 0) in vec3 Position0;


out vec2 texCoord;

void main()
{
	gl_Position =vec4(Position0 , 1.0f);
}