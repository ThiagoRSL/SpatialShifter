// Fragment Shader
//
// Este Fragment Shader amostra a textura com id 'colorTextureSampler' e aplicada 
// esta amostra ao fragmento.
// Nenhum cálculo de iluminação é feito.
// Abril 2016 - Alex Frasson - afrasson@inf.ufsm.br

#version 460

in vec4 vColor;
in vec4 vPosition;

out vec4 result_color;

void main()
{
	if(vColor.d == 0.0f)
		discard();
	result_color = vColor;
}