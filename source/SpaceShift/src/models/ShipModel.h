#pragma once

#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <vector>

#include "../common/Shader.h"
#include "../entities/Entity.h"
#include "RenderModel.h"

#define WINDOW_WIDTH	1000.0
#define WINDOW_HEIGHT	1000.0

class Ship;

class ShipModel : public RenderModel
{

private:
	//void GenerateColliderDebugBuffers(const char* modelPath);
	//void GenerateColliderVertexes();

	int size;
	GLFWwindow* window;

	Ship* Controller;
	//Shaders dos Eixos da Nave
	Shader *AxisShader;
	//Shaders do Modelo da Nave
	Shader* ShipModelShader;
	vec4 ShipModelColor;

	Shader* ParticleUpdateShader;
	Shader* ParticleShader;
	GLuint	ParticlesVaoID;
	GLuint* ParticlesVboID;
	unsigned int ParticleTextureIndex;
	int MaxParticlesNumber;
	float MaxTimeToLive;
	int NextParticleIndex;

	int ParticleNumber;
	vec4* ParticlePosition;
	vec4* ParticleVelocity;
	vec4* ParticleColor;
	float* ParticleTTL;
	int* ParticleIndex;


public:
	ShipModel() { ; }
	ShipModel(Ship* ship);

	// mesh virtual functions
	void Init();
	void Update(double t);
	void GenerateBuffers();
	void Render();
	//void resize(int, int);


	void GenerateParticles();

	void SetScale(vec3 scale) { this->scale = scale; }
	void SetShipModelColor(vec4 ShipModelColor) { this->ShipModelColor = ShipModelColor; }
	void SetShipController(Ship* ship);
	void UpdateLight(double deltaTime);

	mat4 GetMVP() { return this->modelViewProjectionMatrix; }
	mat4 GetRotationMatrix() { return this->rotationMatrix; }
	mat4 GetScaleMatrix() { return this->scaleMatrix; }
	std::vector< vec3 > GetVertexes() { return this->Vertexes; }
	std::vector<std::pair < unsigned int, unsigned int >> GetEdges() { return this->ShipModelShader->Edges; }


};


/*

Uma nave espacial leve pesa entre 40 e 100 toneladas e requer de dez a trinta tripulantes.
Uma nave espacial média pesa entre 100 e 1000 toneladas e requer de vinte a quinhentos tripulantes.
Uma nave espacial pesada pesa mais de 1000 toneladas e requer mais de quinhentos tripulantes para operar.
*/