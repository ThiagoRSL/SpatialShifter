#version 430

uniform sampler2D text;
uniform vec4 TextColor;

in vec2 TexCoords;

out vec4 result_color;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    result_color = TextColor * sampled;
}  