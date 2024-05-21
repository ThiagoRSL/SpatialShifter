#pragma once

#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <iostream>
#include <vector>

#include "TextureManager.h"
#include "GLUtils.h"
#include "../entities/Ship.h"
#include "../colliders/PolyCollider.h"

/*
	Para otimizar o uso de mem�ria na placa de video, devemos garantir que cada modelo seja carregado apenas 1 vez, isso significa que
	entidades que utilizem o mesmo modelo devem utilizar o mesmo shader, passando informa��es como uniformes antes de realizarem cada
	renderiza��o que desejarem. Para isso, essa classe cria dicion�rios para shaders, GARANTINDO que haja apenas uma inst�ncia de shader
	para cada modelo poss�vel. Assim, toda classe que quiser renderizar um modelo, deve buscar se o shader para aquele modelo j� foi 
	criado com GetShader, se sim, pode apenas utilizar ele, se n�o, ela a classe � respons�vel por implementar as fun��es adequadas para
	gerar os buffers e instanciar um shader configurado com eles, depois, usar o AddShader desta classe para garantir que as pr�ximas
	consultas respondam com este shader j� instanciado.
	Um tipo de shader identifica quais ser�o os .vert e .frag (poss�velmente outros shaders com tesselation ou geometry) que ser�o utilizados
	para compilar, definindo tamb�m, obviamente, quais s�o as entradas esperadas para este shader. J� o id do shader identifica qual inst�ncia
	� a desejada, ou seja, existe um tipo de shader, respons�vel por renderizar o modelo das NAVES ESPACIAIS (que seria do tipo SHADER_TYPE_SHIP),
	mas em tempo de execu��o podem ser instanciadas varias vers�es desse shader, cada uma com um tipo de modelo diferente.
*/
enum class ShaderType {
	SHADER_TYPE_TEXT,
	SHADER_TYPE_SHIP,
	SHADER_TYPE_COLLIDER,
	SHADER_TYPE_AXIS,	//TODO: In�til, tirar dps
	SHADER_TYPE_SKILL,
	SHADER_TYPE_PROJECTILE,
	SHADER_TYPE_INTERFACE,
};

class ShaderManager
{


private:

	static ShaderManager* shaderManagerInstance;
	GLFWwindow* window = nullptr;

	ShaderManager();

	std::map<int, Shader*> TextShaderDict;
	std::map<int, Shader*> ShipShaderDict;
	std::map<int, Shader*> AxisShaderDict;
	std::map<int, Shader*> ColliderShaderDict;
	std::map<int, Shader*> InterfaceShaderDict;
	std::map<SkillID, Shader*> SkillShaderDict;
	std::map<ProjectileTypeID, Shader*> ProjectileShaderDict;
	std::map<ShaderType, Shader*> ShaderDict;

	Shader* particleShader;
	Shader* interfaceElementShader;
	Shader* cursorShader;

	//Retorna o dicion�rio correto que armazena o tipo de shader em quest�o.
	std::map<int, Shader*>* GetSelector(ShaderType dict_type_id);

public:

	//Inicia os dicion�rios
	void InitShaderDict() 
	{ 
		TextShaderDict = std::map<int, Shader*>();
		ShipShaderDict = std::map<int, Shader*>();
		AxisShaderDict = std::map<int, Shader*>();
		ColliderShaderDict = std::map<int, Shader*>();
		SkillShaderDict = std::map<SkillID, Shader*>();
		ProjectileShaderDict = std::map<ProjectileTypeID, Shader*>();
		InterfaceShaderDict = std::map<int, Shader*>();
		interfaceElementShader = NULL;
		cursorShader = NULL;
		particleShader = NULL;
	}

	//Adiciona um shader � um dicion�rio
	void AddShader(SkillID skillId, Shader* shader);
	void AddShader(ShaderType typeShader, Shader* shader);
	void AddShader(ProjectileTypeID projectileId, Shader* shader);
	void AddShader(int id, ShaderType typeShader, Shader* shader);

	//Pega um shader do dicion�rio, caso n�o encontre, retorna nulo.
	Shader* GetShader(SkillID skillId);;
	Shader* GetShader(ShaderType typeShader);
	Shader* GetShader(ProjectileTypeID projectileId);
	Shader* GetShader(int id, ShaderType typeShader);
	Shader* GetParticleShader() { return this->particleShader; }
	Shader* GetInterfaceElementShader() { return this->interfaceElementShader; }
	GLuint GetInterfaceElementShaderID() { return this->interfaceElementShader->ID; }
	Shader* GetCursorShader() { return this->cursorShader; }
	GLuint GetCursorShaderID() { return this->cursorShader->ID; }
	void SetInterfaceElementShader(Shader* ieshader) { this->interfaceElementShader = ieshader; }
	void SetCursorShader(Shader* cursorShader) { this->cursorShader = cursorShader; }
	void SetParticleShader(Shader* particleShader) { this->particleShader = particleShader; }
	
	//void AddCollider(AbstractCollider* collider) { RenderList.push_back(entity); }

public:

	static ShaderManager* GetInstance()
	{
		if (shaderManagerInstance == nullptr)
		{
			shaderManagerInstance = new ShaderManager();
			shaderManagerInstance->InitShaderDict();
		}
		return shaderManagerInstance;
	};
};