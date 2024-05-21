// Vertex Shader
//
// Como as tranformções serão aplicadas somente no estágio Tessellation Evaluation Shader, este
// vertex shader apenas passa os vértices de entrada adiante para o Tessellation Control Shader.
//
// Abril 2016 - Alex Frasson - afrasson@inf.ufsm.br

#version 460

uniform vec4 PlayerPosition;

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexColor;
layout(location = 2) in vec2 TexCoord;

out vec4 vPosition;
out vec2 vTexCoord;
out vec3 vColor;

uniform mat3 NormalMatrix;

void main()
{
	vTexCoord = TexCoord;
	vPosition = vec4(VertexPosition, 1.0);
	vPosition = vec4(VertexPosition.x, VertexPosition.y, VertexPosition.z, 1.0);	
    vColor = VertexColor;
}
