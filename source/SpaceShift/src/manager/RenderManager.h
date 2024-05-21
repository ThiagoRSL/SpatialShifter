#pragma once

#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <iostream>
#include <vector>

#include "TextureManager.h"
#include "GLUtils.h"
#include "../entities/Entity.h"
#include "../utils/GlobalVars.h"

class RenderManager
{

private:
	static RenderManager* renderManagerInstance;
	GLFWwindow* window = nullptr;

	RenderManager();
	std::vector<Entity*> RenderList;
	CameraManager* CameraManagerInstance;

public:

	void InitRenderList() { RenderList = std::vector<Entity*>(); }
	void RenderElements();
	void AddRenderable(Entity* entity) { RenderList.push_back(entity); }
	void RemoveRenderable(Entity* entity) { RenderList.erase(std::remove(RenderList.begin(), RenderList.end(), entity), RenderList.end()); }

public:
	float RENDER_DISTANCE;

	static RenderManager* GetInstance()
	{
		if (renderManagerInstance == nullptr)
		{
			renderManagerInstance = new RenderManager();
			renderManagerInstance->InitRenderList();
		}
		return renderManagerInstance;
	};
};