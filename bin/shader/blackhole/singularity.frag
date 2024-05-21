// Fragment Shader
//
// Este Fragment Shader amostra a textura com id 'colorTextureSampler' e aplicada 
// esta amostra ao fragmento.
// Nenhum cálculo de iluminação é feito.
// Abril 2016 - Alex Frasson - afrasson@inf.ufsm.br

#version 460

in vec4 vPosition;
in vec2 vUv;
in vec3 vNormal;
in vec4 vColor;

out vec4 result_color;

void main()
{
	vec3 colorVec = vec3(vColor.x, vColor.y, vColor.z);
	result_color = vec4(colorVec.x, colorVec.y, colorVec.z, 1.0);
}