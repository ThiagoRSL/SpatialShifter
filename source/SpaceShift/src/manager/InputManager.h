#pragma once

#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <vector>
#include <iostream>

#include "GLUtils.h"

#include "../Utils/MathUtils.h"
#include "../Utils/CoordinateTranslateUtils.h"
#include "../manager/TextureManager.h"
#include "../manager/CameraManager.h"
#include "../manager/StageManager.h"
#include "../entities/Ship.h"
#include "../models/interface/CursorModel.h"
#include "../utils/GlobalVars.h"


#define WINDOW_WIDTH	1000.0
#define WINDOW_HEIGHT	1000.0

class InterfaceManager;

class InputManager
{

private:
	static InputManager* instance;
	CameraManager* CameraInstance;
	GLFWwindow* window;
	vec2 windowSize;
	CursorModel Model;

	vec2 mousePosition;
	Ship* DebugEnemy;
	Ship* PlayerShip;

	InputManager();
	InputManager(GLFWwindow* window, int size);
	void VerifyMouseCollision();

public:
	void SetDebugMouse(Ship* s) { DebugEnemy = s; }
	static InputManager* GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new InputManager();
			instance->Init();
		}
		return instance;
	};

	void Init();
	void Update(double deltaTime);
	void KeyboardInput(int key, int scancode, int action, int mods);
	void ProcessMouse(double deltaTime);
	void UpdateMousePosition(float xpos, float ypos);
	void UpdateMouseScroll(float xpos, float ypos);

	void SetWindow(GLFWwindow* window);
	void UpdateWindowSize();
	vec2 GetWindowSize() { return windowSize; }

	void SetPlayerShip(Ship* ps) { this->PlayerShip = ps; }

	
	void RenderCursor();

};