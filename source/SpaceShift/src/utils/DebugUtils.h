#pragma once

#include <vector>
#include <iostream>

#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include "../common/Shader.h"


class DebugUtils
{
	public:
		static double Norm(glm::vec2 v) { return sqrt((v.x * v.x) + (v.y * v.y)); }
		static double Norm(glm::vec3 v) { return sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z)); }
		static double Norm(glm::vec4 v) { return sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z) + (v.w * v.w)); }
		static double Norm(glm::vec2* v) { return sqrt((v->x * v->x) + (v->y * v->y)); }
		static double Norm(glm::vec3* v) { return sqrt((v->x * v->x) + (v->y * v->y) + (v->z * v->z)); }
		static double Norm(glm::vec4* v) { return sqrt((v->x * v->x) + (v->y * v->y) + (v->z * v->z) + (v->w * v->w)); }
		static double Clip(double value, double min, double max) { return (value < min) ? min : ((value > max) ? max : value); }
};