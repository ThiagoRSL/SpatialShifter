#pragma once

#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <vector>

#include <iostream>
#include "TextureManager.h"
#include "../Utils/MathUtils.h"
#include "GLUtils.h"
#include "../utils/GlobalVars.h"

class CameraManager
{

private:
	static CameraManager* instance;
	GLFWwindow* window = nullptr;

	glm::mat4 viewMatrix; 
	glm::mat4 projectionMatrix;
	glm::mat4 orthographicMatrix;
	glm::mat4 mvp;

	//glm::vec3 perspective = glm::vec3(45.0f, 0.0f, 0.0f);
	glm::vec3 perspective = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 70.0f);
	glm::vec3* cameraTarget;
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec3 cameraDirection;
	glm::vec3 cameraRight;
	glm::vec3 cameraUp;
	glm::vec3 direction;
	
	vec3 initialPoint;
	vec3* targetPoint;

	float pitch = 0.0f;
	float yaw = 0.0f;
	float row = 0.0f;

	float size, sizeMinLimit, sizeMaxLimit;
	float nearPlane;
	float farPlane;

	CameraManager();

public:
	static CameraManager* GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new CameraManager();
		}
		return instance;
	};

	// mesh virtual functions
	void InitCamera(GLFWwindow* window);
	void UpdateCameraSettings();
	void ChangeSize(float deltaSize);
	void Update()
	{
		//worldPivot = vec3(this->cameraTarget->x, this->cameraTarget->y, 0);
	}

	mat4 ViewMatrix() { return viewMatrix; }
	mat4 ProjectionMatrix() { return projectionMatrix; }
	mat4 MVP() { return mvp; }
	vec3 Perspective() { return perspective; }
	vec3 WorldPivot() { return vec3(*targetPoint); }
	float GetSize() { return size; }
	float GetSizeRelative() { return size / sizeMaxLimit; }

	void AdjustPitch(float val)
	{ 
		pitch += val;
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		double norm = MathUtils::Norm(cameraPosition);

		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(direction);
	}
	void AdjustYaw(float val)
	{
		yaw += val;
		if (yaw > 360.0f || pitch < -360.0f)
			yaw = 0.0f;

		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(direction);
	}
	void SetTargetPoint(vec3* targetPoint)
	{
		//this->cameraTarget = targetPoint;
		this->targetPoint = targetPoint;
	}

};