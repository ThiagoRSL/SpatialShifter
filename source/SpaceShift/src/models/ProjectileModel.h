#pragma once

#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <vector>

#include "../common/Shader.h"
#include "../entities/Entity.h"
#include "RenderModel.h"

#define WINDOW_WIDTH	1000.0
#define WINDOW_HEIGHT	1000.0

class Projectile;

class ProjectileModel : public RenderModel
{

private:
	//void GenerateColliderDebugBuffers(const char* modelPath);
	//void GenerateColliderVertexes();

	int size;
	GLFWwindow* window;

	Shader* ProjectileShader;
	Projectile* Controller;
	//Shaders dos Eixos da Nave
	Shader *AxisShader;
	//Shaders do Modelo da Nave
	vec4 ProjectileModelColor;




public:
	ProjectileModel() { ; }
	ProjectileModel(Projectile* Projectile);

	// mesh virtual functions
	void Init();
	void Update(double t);
	void GenerateBuffers();
	void Render();
	//void resize(int, int);


	void GenerateParticles();

	void SetScale(vec3 scale) { this->scale = scale; }
	void SetProjectileModelColor(vec4 ProjectileModelColor) { this->ProjectileModelColor = ProjectileModelColor; }
	void SetProjectileController(Projectile* Projectile);
	void UpdateLight(double deltaTime);

	mat4 GetRotationMatrix() { return this->rotationMatrix; }
	mat4 GetScaleMatrix() { return this->scaleMatrix; }
	std::vector< vec3 > GetVertexes() { return this->Vertexes; }
	std::vector<std::pair < unsigned int, unsigned int >> GetEdges() { return this->ProjectileShader->Edges; }


};


/*

Uma nave espacial leve pesa entre 40 e 100 toneladas e requer de dez a trinta tripulantes.
Uma nave espacial média pesa entre 100 e 1000 toneladas e requer de vinte a quinhentos tripulantes.
Uma nave espacial pesada pesa mais de 1000 toneladas e requer mais de quinhentos tripulantes para operar.
*/