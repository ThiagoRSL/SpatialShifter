#version 430

uniform sampler2D TextureSampler;

in vec2 vTex;

out vec4 result_color;

void main()
{
	result_color = texture2D(TextureSampler, vTex);
	//result_color = vec4(0.0f,1.0f,0.0f,1.0f);
}