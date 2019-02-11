#version 330

uniform vec4 color;
out vec4 fragColor;

in vec2 texCoord;

uniform sampler2D texture0;

void main()
{
	fragColor =  texture(texture0,texCoord);
}