#pragma once

#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <vector>

#include "../common/Shader.h"
#include "../entities/Entity.h"
#include "RenderModel.h"

#define WINDOW_WIDTH	1000.0
#define WINDOW_HEIGHT	1000.0


class ColliderModel : public RenderModel
{

protected:
	Entity* Controller;
	Shader* ColliderShader;

public:
	ColliderModel() { ; }
	ColliderModel(Entity* Controller);

	void LoadCollider(const char* colliderPath);
	void SetController(Entity* controller);
	void Init();
	void Update(double t);
	void Render();
	void GenerateBuffers();
	
};

/*

Uma nave espacial leve pesa entre 40 e 100 toneladas e requer de dez a trinta tripulantes.
Uma nave espacial média pesa entre 100 e 1000 toneladas e requer de vinte a quinhentos tripulantes.
Uma nave espacial pesada pesa mais de 1000 toneladas e requer mais de quinhentos tripulantes para operar.
*/