
#include "CameraManager.h"
using namespace std;

CameraManager* CameraManager::instance = nullptr;


CameraManager::CameraManager()
{
	initialPoint = vec3(0.f);
	targetPoint = &initialPoint;

	nearPlane = 0.1f;
	farPlane = 40.0f;
	size = 50.0f; // O tamanho da visão isométrica
	sizeMinLimit = 20.0f;
	sizeMaxLimit = 80.0f;
}

void CameraManager::InitCamera(GLFWwindow* window)
{
	this->window = window;
	viewMatrix = glm::lookAt(
		vec3(0.0f, 0.0f, 20.0f),
		vec3(0.0f, 0.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f));

	int w, h;
	glfwGetFramebufferSize(window, &w, &h);
	// Crie a matriz de projeção ortográfica
	// Agora você pode multiplicar sua matriz de modelo pela matriz de projeção e visualização
	//mvp = mat4() * projectionMatrix * viewMatrix;

	this->UpdateCameraSettings();
}


void CameraManager::UpdateCameraSettings()
{
	int w, h;
	glfwGetFramebufferSize(this->window, &w, &h);
	if (GlobalVars::PERSPECTIVE)
	{
		projectionMatrix = glm::perspective(glm::radians(90.0f), (float)w / (float)h, 0.1f, 100.0f);
	}
	else
	{
		//projectionMatrix = glm::perspective(glm::radians(90.0f), (float)w / (float)h, 0.1f, 100.0f);
		float aspectRatio = static_cast<float>(w) / static_cast<float>(h);
		projectionMatrix = glm::ortho(-size * aspectRatio, size * aspectRatio, -size, size, nearPlane, farPlane);
	}
	this->mvp = mat4() * projectionMatrix * viewMatrix;
}

void CameraManager::ChangeSize(float deltaSize)
{
	this->size += deltaSize;
	if (this->size > this->sizeMaxLimit)
	{
		this->size = this->sizeMaxLimit;
	}
	else if (this->size < this->sizeMinLimit)
	{
		this->size = this->sizeMinLimit;
	}
	UpdateCameraSettings();
}

