#pragma once

#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <vector>

#include "../../common/Shader.h"
#include "../RenderModel.h"


class CursorModel : public RenderModel
{

private:
	Shader* CursorShader;
	int TextureIndex;

	vec2 mousePosition;

	void GenerateBuffers();

public:
	CursorModel() { ; }

	// mesh virtual functions
	void Init();
	void Update(double t);
	void Resize();
	void Render();
	void SetCursorPosition(vec2 mousePosition) { this->mousePosition = mousePosition; }

};


/*

Uma nave espacial leve pesa entre 40 e 100 toneladas e requer de dez a trinta tripulantes.
Uma nave espacial média pesa entre 100 e 1000 toneladas e requer de vinte a quinhentos tripulantes.
Uma nave espacial pesada pesa mais de 1000 toneladas e requer mais de quinhentos tripulantes para operar.
*/