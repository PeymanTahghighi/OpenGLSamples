#version 330

out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D texture0;
uniform sampler2D texture1;

const float offset = 1.0f/300.0f;
void main()
{
	vec2 offset[9] = vec2[](
		vec2(-offset,offset), //topleft
		vec2(0.0f,offset),//topcenter
		vec2(offset,offset),//topright
		vec2(-offset,0.0f),//centerleft
		vec2(0.0f,0.0f),//middle
		vec2(offset,0.0f),//centerright
		vec2(-offset,-offset),//downleft
		vec2(0.0f,-offset),//downmiddle
		vec2(offset,-offset)
	);
	
	float kernel[9] = float[](
		0,0,0,
		0,1,0,
		0,0,0
	);
	
	vec3 sampleTexture[9];
	
	for(int i=0;i<9;++i)
	{
		sampleTexture[i] = texture(texture1,texCoord.st + offset[i],2).rgb;
	}
	//
	vec3 color = vec3(0.0f);
	//
	for(int i=0;i<9;++i)
		color+=sampleTexture[i]*kernel[i];
	
	//float gamma = 2.2f;

	//float exposure = 1.0f;
	vec4 hdrColor = texture(texture1,texCoord);
	//hdrColor.rgb  = vec3(1.0f) - exp(-hdrColor.rgb * exposure);
	//vec4 bloomColor = texture(texture1,texCoord);
	//vec4 finalColr = hdrColor + bloomColor;

	//gamma correction
	//hdrColor.rgb=pow(hdrColor.rgb,vec3(1/gamma));

	FragColor = vec4(hdrColor.rgb,1.0f);
	
}