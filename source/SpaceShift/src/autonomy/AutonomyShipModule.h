#pragma once

#include <GL/glew.h>

#include <GLFW\glfw3.h>
#include <vector>
#include <deque>

#include "../Entities/Entity.h"
#include "../utils/GlobalVars.h"
#include "../models/ShipModel.h"
#include "../colliders/PolyCollider.h"
#include "../scripts/skills/SkillAbstract.h"

#define WINDOW_WIDTH	1000.0
#define WINDOW_HEIGHT	1000.0

class Ship;
class Task;

enum class AutonomyTask {
	STABILIZE,
	ROTATE,
};

enum class AutonomyBehavior {
	NONE,
	STABILIZE,
	WANDER,
	SEARCH,
	SEEK,
	SLAM,
	COMBAT,
};

class AutonomyShipModule
{

private:
	std::deque<Task*> Tasks;
	Task* CurrentTask;
	AutonomyBehavior CurrentBehavior;

	bool starting;
	Ship* Agent;
	float awareRadius;
	float autonomyShotCooldown;

	vec3 AimedSpeed;
	vec3* EnemyPosition;
	vec2 TargetPosition;
	vec2 SeekPosition;
	float SpeedLimit = 60;
	float StabilizedSpeedTolerance = 0.1f;




public:
	AutonomyShipModule() { ; }
	AutonomyShipModule(Ship* agent, float aware_radius, vec3* enemyPosition);

	void Init(ShipTypeId shipTypeId);
	void Update(double t);
	void Destroy();
	void SeekTo(vec3 targetLocation);

	bool Seeking(double t);
	bool Wandering(double t);
	bool Combating(double t);

	bool RotateToAngle(float angle, float angleTolerance);
	bool MovingTowards(float angle, float angleTolerance);
	bool Stabilize();
	bool IsStabilized();
	void SetBehaviour(AutonomyBehavior behavior) { this->CurrentBehavior = behavior; }


	void ShutdownAutonomy();
	void ClearTasks();
	void AddTaskStart(Task* t);
	void AddTaskEnd(Task* t);
	Task* PeekTask() 
	{ 
		if (Tasks.empty())
			return nullptr; 
		else return Tasks.front();
	}
	
	Task* FetchTask() {
		if (Tasks.empty()) return nullptr;  

		Task* task = Tasks.front();  
		Tasks.pop_front();           
		return task;                  
	}
};


/*
Uma nave espacial leve pesa entre 40 e 100 toneladas e requer de dez a trinta tripulantes.
Uma nave espacial média pesa entre 100 e 1000 toneladas e requer de vinte a quinhentos tripulantes.
Uma nave espacial pesada pesa mais de 1000 toneladas e requer mais de quinhentos tripulantes para operar.
*/