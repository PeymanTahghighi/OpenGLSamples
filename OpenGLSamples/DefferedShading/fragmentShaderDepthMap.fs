#version 330

in vec4 FragPos;

uniform vec3 lightPos;
uniform float far_plane;

void main()
{	
	float distance = length(lightPos - FragPos.xyz);
	gl_FragDepth = distance / far_plane;
}