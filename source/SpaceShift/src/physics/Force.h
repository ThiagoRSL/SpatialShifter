#pragma once

#include <vector>
#include <iostream>

#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include "GLUtils.h"


class Force
{

private:
	int source;
	double direction;
	double intensity;
	glm::vec3 force;
	glm::vec3 target;

public:
	Force();
	Force(glm::vec3 v) { SetAsVec(v); }

	int GetSource() { return this->source; }
	glm::vec3 GetVec() { return force; }
	glm::vec3* GetVecRef() { return &force; }
	glm::vec3 GetTargetPoint() { return target; }

	void SetSource(int source);
	void SetAsVec(glm::vec3 v);
	void SetAsVec(float x, float y, float z);
	void SetTargetPoint(glm::vec3 vec) { target = vec; }
	void SetTargetPoint(float x, float y, float z) { target = glm::vec3(x, y, z); }
};

