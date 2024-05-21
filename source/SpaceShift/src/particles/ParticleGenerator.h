#pragma once

#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <vector>

#include "../common/Shader.h"
#include "../physics/Force.h"
#include <iostream>
#include "TextureManager.h"
#include "../common/ObjectLoader.h"
#include "../Utils/MathUtils.h"
#include "GLUtils.h"
#include "Particle.h"


#define WINDOW_WIDTH	1000.0
#define WINDOW_HEIGHT	1000.0

class ParticleGenerator
{

public:
	ParticleGenerator();
	ParticleGenerator(GLFWwindow* window, int size);

	void init();
	void Update(double t);
	void Render();



protected:
	int size;
	double mass; //F = m . a	-> F/m = a

	std::vector<Particle*> Particles = std::vector<Particle*>();
	vec3 forcesSum;
	vec3 acceleration;
	vec3 speed;
	vec3 position;

	vec3 rotation;
	vec3 direction;


private:
	GLFWwindow* window;

};