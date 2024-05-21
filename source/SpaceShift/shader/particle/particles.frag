#version 430

uniform sampler2D TextureSampler;
in	vec2 TexCoords;
in float gLiving;
in vec4 gColor;

out vec4 result_color;

void main()
{
	result_color = texture2D(TextureSampler, TexCoords);

	if(result_color.w == 0.0f) discard;
	
	result_color = gColor;
	result_color.w = gLiving;
}