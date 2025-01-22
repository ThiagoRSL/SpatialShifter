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
#include "../entities/Ship.h"
#include "RenderManager.h"
#include "UpdateManager.h"
#include "CollisionManager.h"

class StageManager
{

private:
	StageManager();
	static StageManager* StageManagerInstance;
	int KillCounter;
	int EnemyCounter;
	bool BossSpawned;
	Ship* PlayerShip;
	std::vector<Ship*> Enemies;

public:

	void SpawnEnemy();
	void SpawnBoss();
	void KillCountIncrement() { KillCounter++; }
	void SetKillCount(int v) { KillCounter = v; }
	void SetPlayer(Ship* PlayerShip) { this->PlayerShip = PlayerShip; }
	void ShipDestroyed(Ship* ship);
	void UpdateStage(double t);
	void Init();

public:

	static StageManager* GetInstance()
	{
		if (StageManagerInstance == nullptr)
		{
			StageManagerInstance = new StageManager();
		}
		return StageManagerInstance;
	};
};