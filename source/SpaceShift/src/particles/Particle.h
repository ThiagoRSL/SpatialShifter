#pragma once

#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <vector>

#include "../common/Shader.h"
#include "../physics/Force.h"
#include "../entities/Entity.h"
#include <iostream>
#include "TextureManager.h"
#include "../common/ObjectLoader.h"
#include "../Utils/MathUtils.h"
#include "Particle.h"
#include "GLUtils.h"


#define WINDOW_WIDTH	1000.0
#define WINDOW_HEIGHT	1000.0

class Particle : public Entity
{

public:
	Particle();
	Particle(GLFWwindow* window, int size);

	void init();
	void Update(double t);
	void Render() { ; }



protected:
	double timeToLive;
	vec4 color;


private:
	GLFWwindow* window;

};