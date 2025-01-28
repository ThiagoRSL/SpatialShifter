#pragma once

#include <GL/glew.h>

#include <GLFW\glfw3.h>
#include <vector>

#include "../AutonomyShipModule.h"
#include "../../Entities/Entity.h"
#include "../../utils/GlobalVars.h"
#include "../../models/ShipModel.h"
#include "../../colliders/PolyCollider.h"

enum TaskState {
	CREATED,
	STARTING,
	EXECUTING,
	STOPED,
	ENDED,
};

class Task
{
	public:
		AutonomyShipModule* Autonomy;
		TaskState State;

		virtual void AddedQeue(AutonomyShipModule* Autonomy) { this->Autonomy = Autonomy; }
		virtual void Create() { return; }
		virtual void Start() { this->State = TaskState::EXECUTING; }
		virtual bool CheckRequirements() { return true; }
		virtual void Run(double t) = 0;
		virtual void End() { this->State = TaskState::ENDED; }
		virtual void Abort() { this->End(); }
};

class StabilizeToTask : public Task
{
public:
	StabilizeToTask() { ; }

	void Run(double t)
	{
		if (this->Autonomy->Stabilize())
			this->End();
	}
};

class SeekTask : public Task
{
	vec3 targetPosition;

public:
	SeekTask(vec3 targetPosition)
	{
		this->targetPosition = targetPosition;
	}


	void Run(double t)
	{
		this->Autonomy->SeekTo(targetPosition);
		if(this->Autonomy->Seeking(t)) 
			End();
	} 

	void End()
	{
		Task::End();
	}
};

class RotateToTask : public Task
{
	float angle;
	float angleTolerance;

public:
	RotateToTask(float angle, float angleTolerance)
	{
		this->angle = angle;
		this->angleTolerance = angleTolerance;
	}

	void Set(float angle, float angleTolerance)
	{
		this->angle = angle;
		this->angleTolerance = angleTolerance;
	}

	void Run(double t)
	{
		if (this->Autonomy->RotateToAngle(angle, angleTolerance))
			this->End();
	}
};

class ShutdownAutonomyTask : public Task
{
	// Numero de vezes que repassa a task até conseguir
	int Tries;

public:
	ShutdownAutonomyTask(int tries) { this->Tries = tries; }

	void Run(double t)
	{		
		if (this->Autonomy->PeekTask() != nullptr)
		{
			printf("\n Autonomia ia ser desligada, mas a o shutdown não era a ultima task.");
			
			if (this->Tries > 0)
			{
				ShutdownAutonomyTask* t = new ShutdownAutonomyTask(this->Tries - 1);
				this->Autonomy->AddTaskEnd(t);
			}

			this->End();
			return;
		}

		this->End();
		this->Autonomy->ShutdownAutonomy();
	}
};

class PatrolTask : public Task
{
	std::vector<vec3> PatrollingPath;
	int currentIndex;
	// Numero de vezes que repassa a task até conseguir

public:

	PatrolTask(std::vector<vec3> patrollingPath)
	{
		this->currentIndex = 0;
		this->PatrollingPath = patrollingPath;
	}

	void Start()
	{
		this->Autonomy->SeekTo(PatrollingPath.at(this->currentIndex));
		this->State = TaskState::EXECUTING;
	}

	void Run(double t)
	{
		if (this->Autonomy->Seeking(t))
		{
			this->currentIndex += 1;

			if (this->currentIndex >= PatrollingPath.size())
				this->currentIndex = 0;

			this->Autonomy->SeekTo(PatrollingPath.at(currentIndex));
		}
	}
};

class FollowTask : public Task
{
	Entity* Target;

public:

	FollowTask(Entity* target) { this->Target = target; }

	void Run(double t)
	{
		vec3 position = CalculatePositionFollower();
		this->Autonomy->SeekTo(position);
		if (this->Autonomy->Seeking(t))
		{
			return;
		}
	}

	vec3 CalculatePositionFollower()
	{
		//fmod(Target->GetRotation().z + 180.0f, 360.0f);
		vec3 position = Target->GetPosition() + (Target->GetDirection() * vec3(-50.0f, -50.0f, 0.0f));
		return position;
	}
};