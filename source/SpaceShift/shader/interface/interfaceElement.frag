#version 430
uniform sampler2D BackgroundTexture;
uniform vec4 BackgroundColor;

in vec2 TexCoords;

out vec4 result_color;

void main()
{    
	result_color = texture2D(BackgroundTexture, TexCoords);
}  