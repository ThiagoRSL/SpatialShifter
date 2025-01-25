
#include "CameraManager.h"
using namespace std;

CameraManager* CameraManager::instance = nullptr;


CameraManager::CameraManager()
{
	initialPoint = vec3(0.f);
	targetPoint = &initialPoint;

	nearPlane = 0.1f;
	farPlane = 40.0f;
	size = 50.0f; // O tamanho da vis�o isom�trica
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
	// Crie a matriz de proje��o ortogr�fica
	// Agora voc� pode multiplicar sua matriz de modelo pela matriz de proje��o e visualiza��o
	//mvp = mat4() * projectionMatrix * viewMatrix;

	this->UpdateCameraSettings();
}


void CameraManager::UpdateCameraSettings()
{
	int w, h;
	glfwGetFramebufferSize(this->window, &w, &h);

	// Fator de zoom
	ZoomFactor = 1.0f + (size * 0.01f); // Ajuste o valor conforme necess�rio

	// Ajusta o nearPlane e farPlane com base no zoom
	nearPlane = 0.1f * ZoomFactor;   // N�o deixe o nearPlane muito pequeno
	farPlane = 100.0f * ZoomFactor;  // Ajuste o farPlane para um valor adequado


	if (GlobalVars::PERSPECTIVE)
	{
		projectionMatrix = glm::perspective(glm::radians(90.0f), (float)w / (float)h, 0.1f, 100.0f);
	}
	else
	{
		//projectionMatrix = glm::perspective(glm::radians(90.0f), (float)w / (float)h, 0.1f, 100.0f);
		float aspectRatio = static_cast<float>(w) / static_cast<float>(h);
		projectionMatrix = glm::ortho(-size * aspectRatio * ZoomFactor, size * aspectRatio * ZoomFactor,
			-size * ZoomFactor, size * ZoomFactor, nearPlane, farPlane);
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


