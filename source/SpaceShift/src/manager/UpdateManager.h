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
#include "../scripts/skills/SkillAbstract.h"
#include "RenderManager.h"

class UpdateManager
{

private:
	static UpdateManager* updateManagerInstance;
	GLFWwindow* window = nullptr;
	double lastDeltaTime;

	UpdateManager();
	std::vector<Entity*> UpdateList;
	std::vector<SkillAbstract*> SkillList;

	//Tempo fixo entre frames pro update.
	static double FIXED_DELTA_TIME;
	static double FRAME_TIME;
	static int MAX_FPS;

	float timeCounter;
	double lastFrameTime;
	int frame;


public:

	void InitLists() { UpdateList = std::vector<Entity*>(); SkillList = std::vector<SkillAbstract*>(); timeCounter = 0.0f; }
	void CheckTime(double time) { if (time >= lastFrameTime + FRAME_TIME) { lastFrameTime = time; frame++; }; }
	void UpdateElements(double deltaTime);
	void AddUpdatable(Entity* entity)		{ UpdateList.push_back(entity); }
	void AddUpdatable(SkillAbstract* skill) { SkillList.push_back(skill); }
	void RemoveUpdatable(Entity* e)			{ UpdateList.erase(std::remove(UpdateList.begin(), UpdateList.end(), e), UpdateList.end());	}
	void RemoveUpdatable(SkillAbstract* s)  { SkillList.erase(std::remove(SkillList.begin(), SkillList.end(), s), SkillList.end()); }
	//TODO: Remover skills também.
	double GetLastDeltaTime() { return lastDeltaTime; }
	float GetTimeCounter() { return timeCounter; }
	int GetLastFrame() { return frame; }

public:

	static UpdateManager* GetInstance()
	{
		if (updateManagerInstance == nullptr)
		{
			updateManagerInstance = new UpdateManager();
			updateManagerInstance->InitLists();
		}
		return updateManagerInstance;
	};
};