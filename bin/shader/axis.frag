
#version 430

//in vec2 gsTexCoord;
//in vec3 gsNormal;
//in vec3 gsColor;

in vec4 vPosition;
in vec4 vColor;

out vec4 result_color;

void main()
{
	result_color = vColor;
}