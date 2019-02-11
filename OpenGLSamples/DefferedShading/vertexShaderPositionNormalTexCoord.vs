#version 330

layout (location = 0) in vec3 Position0;
layout (location = 1) in vec3 Normal0;
layout (location = 2) in vec2 texCoord0;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

out vec2 texCoord;
out vec3 worldPosition;
out vec3 normalWorld;
out vec2 zEye;



void main()
{
	worldPosition = vec4(view * model*vec4(Position0,1.0f)).xyz;
	normalWorld = mat3(inverse(transpose(model))) * Normal0;
	normalWorld = normalize(normalWorld);
	gl_Position = projection *  vec4(worldPosition,1.0f);
	zEye.xy = gl_Position.zw;
	texCoord = texCoord0;
}