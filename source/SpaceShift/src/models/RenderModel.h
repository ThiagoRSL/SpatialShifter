#pragma once

#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <vector>

#include "../common/Shader.h"

#define WINDOW_WIDTH	1000.0
#define WINDOW_HEIGHT	1000.0

class RenderModel
{

protected:
	//std::vector<std::pair<unsigned int, unsigned int>> Edges;

	// Crie a matriz de escala
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
	glm::mat4 modelMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::mat4 modelViewMatrix;
	glm::mat4 rotationMatrix;

	vec3 scale;
	vec3 position;
	vec3 rotation;
	vec3 direction;

public:

	RenderModel() { ; }

	// mesh virtual functions
	virtual void Init() { return; }
	virtual void Update(double t) = 0;
	virtual void Render() = 0;
	//virtual void resize(int, int) = 0;

};


/*
Uma nave espacial leve pesa entre 40 e 100 toneladas e requer de dez a trinta tripulantes.
Uma nave espacial média pesa entre 100 e 1000 toneladas e requer de vinte a quinhentos tripulantes.
Uma nave espacial pesada pesa mais de 1000 toneladas e requer mais de quinhentos tripulantes para operar.
*/