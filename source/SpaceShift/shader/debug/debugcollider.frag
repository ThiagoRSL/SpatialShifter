
#version 430

uniform vec4 DebugColor;
in vec4 vColor;

out vec4 result_color;

void main()
{
	result_color = DebugColor;
}