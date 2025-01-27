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
};

class StabilizeToTask : public Task
{
public:
	StabilizeToTask()
	{

	}

	void Created()
	{

	}

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

	bool CheckRequirements()
	{
		if (this->Autonomy->IsStabilized())
			return true;

		StabilizeToTask* t = new StabilizeToTask();
		this->Autonomy->AddTaskStart(t);
		return false;
	}

	void Run(double t)
	{
		this->Autonomy->SeekTo(targetPosition);
		if(this->Autonomy->Seeking(t)) 
			End();
	} 
};

class RotateToTask : public Task
{
	float angle;
	float angleTolerance;

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
