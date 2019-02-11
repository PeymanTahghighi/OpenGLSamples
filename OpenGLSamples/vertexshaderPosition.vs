#version 330

layout (location = 0) in vec3 Position0;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;


out vec3 texCoord;
void main()
{
	vec4 pos = projection * view * model * vec4(Position0 , 1.0f);
	gl_Position = pos.xyww;
	texCoord = Position0;
}