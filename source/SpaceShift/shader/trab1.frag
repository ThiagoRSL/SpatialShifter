// Fragment Shader
//
// Este Fragment Shader amostra a textura com id 'colorTextureSampler' e aplicada 
// esta amostra ao fragmento.
// Nenhum cálculo de iluminação é feito.
// Abril 2016 - Alex Frasson - afrasson@inf.ufsm.br

#version 460

uniform sampler2D colorTextureSampler;
uniform vec4 PlayerPosition;
uniform float RenderDistance;
uniform bool UseTexture;

uniform vec3 LightDir1;
uniform vec3 LightDir2;
uniform vec3 LightColor1;
uniform vec3 LightColor2;
uniform bool FirstLightSourceOn;
uniform bool SecondLightSourceOn;

in vec2 gsTexCoord;
in vec3 gsNormal;
in vec3 gsColor;

out vec4 result_color;

void main()
{
	vec3 normal = normalize(gsNormal);
	float intensity1;
	float intensity2;
	
	vec4 color;

	if(UseTexture)
		color = texture2D(colorTextureSampler, gsTexCoord);
	else
		color = vec4(gsColor, 1.0f);

	intensity1 = max(dot(LightDir1, -normal), 0.1); 
	intensity2 = max(dot(LightDir2, -normal), 0.1); 
	
	//Iluminação da Luz 1
	if(FirstLightSourceOn)
	{		
		if (intensity1 > 0.98)
			color = color * vec4(0.9,0.9,0.9,1.0) * vec4(LightColor1, 1.0f) * vec4(intensity1);//vec4(intensity1, intensity1, intensity1, 1.0);
		else if (intensity1 > 0.5)
			color = color * vec4(0.4,0.4,0.4,1.0) * vec4(LightColor1, 1.0f) * vec4(intensity1);//vec4(intensity1, intensity1, intensity1, 1.0);	
		else if (intensity1 > 0.25)
			color = color * vec4(0.2,0.2,0.2,1.0) * vec4(LightColor1, 1.0f) * vec4(intensity1);//vec4(intensity1, intensity1, intensity1, 1.0);
		else
			color = color * vec4(0.1,0.1,0.1,1.0) * vec4(LightColor1, 1.0f) * vec4(intensity1);//vec4(intensity1, intensity1, intensity1, 1.0);
	}
	//Iluminação da Luz 2
	if(SecondLightSourceOn)
	{		
		if (intensity2 > 0.98)
			color = color * vec4(0.9,0.9,0.9,1.0) * vec4(LightColor2, 1.0f) *  vec4(intensity2);//vec4(intensity2, intensity2, intensity2, 1.0);
		else if (intensity2 > 0.5)
			color = color * vec4(0.4,0.4,0.4,1.0) * vec4(LightColor2, 1.0f) *  vec4(intensity2);//vec4(intensity2, intensity2, intensity2, 1.0);	
		else if (intensity2 > 0.25)
			color = color * vec4(0.2,0.2,0.2,1.0) * vec4(LightColor2, 1.0f) *  vec4(intensity2);//vec4(intensity2, intensity2, intensity2, 1.0);
		else
			color = color * vec4(0.1,0.1,0.1,1.0) * vec4(LightColor2, 1.0f) * vec4(intensity2);//vec4(intensity2, intensity2, intensity2, 1.0);
	}
		
	result_color = vec4(color.x, color.y, color.z, 1.0f);

	if(distance(gl_FragCoord.xyz,PlayerPosition.xyz) < 10)
	{
		result_color = vec4(1.0, 0.0, 0.0, 1.0);
	}
	result_color = vec4(1.0, 0.0, 0.0, 1.0);
}