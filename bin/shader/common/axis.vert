// Vertex Shader
//
// Como as tranformções serão aplicadas somente no estágio Tessellation Evaluation Shader, este
// vertex shader apenas passa os vértices de entrada adiante para o Tessellation Control Shader.
//
// Abril 2016 - Alex Frasson - afrasson@inf.ufsm.br

#version 460

uniform mat4 MVP;
uniform vec3 Position;

layout(location = 0) in vec3 Vertex;	// Vertex
layout(location = 1) in vec4 Color;		// Colors

out vec4 vColor;
out vec4 vPosition;

void main()
{
	vPosition = MVP * vec4(Position.x+Vertex.x, Position.y+Vertex.y, Position.z+Vertex.z, 1.0);
	gl_Position = vPosition;
	vColor = Color;
}
