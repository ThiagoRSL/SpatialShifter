
#include "CameraManager.h"
using namespace std;

CameraManager* CameraManager::instance = nullptr;


CameraManager::CameraManager()
{
	initialPoint = vec3(0.f);
	targetPoint = &initialPoint;
}

void CameraManager::InitCamera(GLFWwindow* window)
{
	viewMatrix = glm::lookAt(
		vec3(0.0f, 0.0f, 20.0f),
		vec3(0.0f, 0.0f, 0.0f), 
		vec3(0.0f, 1.0f, 0.0f));

	int w, h;
	glfwGetFramebufferSize(window, &w, &h);
	// Crie a matriz de proje��o ortogr�fica
	// Agora voc� pode multiplicar sua matriz de modelo pela matriz de proje��o e visualiza��o
	//mvp = mat4() * projectionMatrix * viewMatrix;

	if (GlobalVars::PERSPECTIVE)
	{

		projectionMatrix = glm::perspective(glm::radians(90.0f), (float)w / (float)h, 0.1f, 100.0f);
	}
	else
	{
		glfwGetFramebufferSize(window, &w, &h);
		//projectionMatrix = glm::perspective(glm::radians(90.0f), (float)w / (float)h, 0.1f, 100.0f);
		float nearPlane = 0.1f;
		float farPlane = 40.0f;
		float aspectRatio = static_cast<float>(w) / static_cast<float>(h);
		float size = 30.0f; // O tamanho da vis�o isom�trica
		projectionMatrix = glm::ortho(-size * aspectRatio, size * aspectRatio, -size, size, nearPlane, farPlane);
	}

	mvp = mat4() * projectionMatrix * viewMatrix;

}


