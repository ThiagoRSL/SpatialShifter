#pragma once

#include <vector>
#include <iostream>

#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include "GLUtils.h"

#include "Force.h"

class Entity;

class SpacePhysics
{
	private:
		SpacePhysics();

	public:
		static void EntityCollision(Entity& e1, Entity& e2, glm::vec3 collisionPoint, float deltaTime);
};

