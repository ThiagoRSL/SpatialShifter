#pragma once

#include <vector>
#include <iostream>
#include <random>

#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "GLUtils.h"

class MathUtils
{
	public:
		static const double PI;

	public:
		static double Norm(glm::vec2 v) { return sqrt((v.x * v.x) + (v.y * v.y)); }
		static double Norm(glm::vec3 v) { return sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z)); }
		static double Norm(glm::vec4 v) { return sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z) + (v.w * v.w)); }
		static double Norm(glm::vec2* v) { return sqrt((v->x * v->x) + (v->y * v->y)); }
		static double Norm(glm::vec3* v) { return sqrt((v->x * v->x) + (v->y * v->y) + (v->z * v->z)); }
		static double Norm(glm::vec4* v) { return sqrt((v->x * v->x) + (v->y * v->y) + (v->z * v->z) + (v->w * v->w)); }
		static double Clip(double value, double min, double max) { return (value < min) ? min : ((value > max) ? max : value); }

		static float DegAngleBetweenFull(glm::vec2 a, glm::vec2 b)
		{
			glm::vec2 diff = b - a;
			return (std::atan2(diff.y, diff.x) * (180.0 / MathUtils::PI)) + 180.0f;
		}
		static float AbsDegAngleBetween(glm::vec2 a, glm::vec2 b)
		{
			glm::vec2 diff = b - a;
			return std::fabs(std::atan2(diff.y, diff.x) * (180.0 / MathUtils::PI));
		}
		static float DistanceBetween(glm::vec2 a, glm::vec2 b)
		{
			glm::vec2 diff = b - a;
			return std::fabs(sqrtf(pow(diff.x, 2) + pow(diff.y, 2)));
		}
		static float RandomBetween(float a, float b)
		{
			std::random_device rd;  // Dispositivo de geração de número aleatório
			std::mt19937 gen(rd()); // Gerador de número aleatório baseado em Mersenne Twister
			std::uniform_real_distribution<> dis(a, b); // Distribuição uniforme

			float random_number = dis(gen);
			return random_number;
		}

		//1 -> horario | -1 -> anti-horario | 0 -> colinear
		//int GetOrientation(float x0, float y0, float x1, float y1, float x2, float y2);
		static int GetOrientation(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3);

		//bool Intercept(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3);
		static bool HasInterception(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4);

		static glm::vec2 FindIntersection(glm::vec2 p1, glm::vec2 q1, glm::vec2 p2, glm::vec2 q2);

		static double AnimationSmoothTime(double value);
};