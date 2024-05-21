
#version 430

uniform vec4 Color;

in vec2 gUv;
in vec3 gNormal;
in vec4 gColor; 

out vec4 result_color;

void main()
{
	//vec3 colorVec = vec3(gColor.x, gColor.y, gColor.z);
	result_color = Color; //vec4(colorVec.x, colorVec.y, colorVec.z, 1.0);
}