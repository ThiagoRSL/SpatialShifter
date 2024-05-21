#include "AutonomyShipModule.h"
#include "../Entities/Ship.h"

AutonomyShipModule::AutonomyShipModule(Ship* agent, float awareRadius, vec3* enemyPosition)
{
	starting = false;
	Agent = agent;
	this->awareRadius = awareRadius;
	this->shotCooldown = 2.0f;

	State = ShipAutonomyState::WANDER;
	WanderToPosition = vec2(Agent->GetPosition());
	EnemyPosition = enemyPosition;
}

void AutonomyShipModule::Update(double t)
{
	if (MathUtils::Norm(*EnemyPosition - Agent->GetPosition()) < this->awareRadius)
	{
		Agent->SetAngularSpeed(vec3(0.0f));
		Agent->SetLinearSpeed(vec3(0.0f));
		this->State = ShipAutonomyState::SHOOT;
	}
	switch (this->State) {
		case ShipAutonomyState::WANDER:
			Wandering(t);
			break;
		case ShipAutonomyState::SHOOT:
			Shooting(t);
			break;
	}
}

void AutonomyShipModule::Wandering(double t)
{
	vec2 agentPos = vec2(Agent->GetPosition());
	float distanceToTarget = MathUtils::Norm(WanderToPosition - agentPos);

	// Se tiver próximo vaga para outra área.
	if (MathUtils::Norm(WanderToPosition - agentPos) < 5.0f)
	{
		float rx = 10.0f - static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 20));
		float ry = 10.0f - static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 20));
		WanderToPosition = vec2(agentPos.x+rx, agentPos.y+ry);
	}

	//Calcula a direção que quer ir
	glm::vec3 direction = glm::normalize(vec3(WanderToPosition.x - agentPos.x, WanderToPosition.y - agentPos.y, 0.0f));
	float angle = - glm::degrees(atan2(direction.x, direction.y));
	float angleTolerance = 2.0f;

	if (Agent->GetRotation().z > angle + angleTolerance || Agent->GetRotation().z < angle - angleTolerance)
	{
		if (angle < Agent->GetRotation().z)
		{
			Agent->SetAngularSpeed(vec3(0.0f, 0.0f, -25.0f));
		}
		else
		{
			Agent->SetAngularSpeed(vec3(0.0f, 0.0f, +25.0f));
		}
		return;
	}
	else
	{
		Agent->SetAngularSpeed(vec3(0.0f));
	}

	if (distanceToTarget > 5.0f)
	{
		Agent->SetLinearSpeed(direction*1.0f * distanceToTarget);
		return;
	}
	else
	{
		Agent->SetLinearSpeed(vec3(0.0f));
	}
	
}


void AutonomyShipModule::Shooting(double t)
{
	vec3 targetPos = vec3(*EnemyPosition);
	vec3 agentPos = Agent->GetPosition();

	float distanceToTarget = MathUtils::Norm(targetPos - agentPos);

	//Calcula a direção que quer ir
	glm::vec3 direction = glm::normalize(vec3(targetPos.x - agentPos.x, targetPos.y - agentPos.y, 0.0f));
	float angle = -glm::degrees(atan2(direction.x, direction.y));
	float angleTolerance = 2.0f;

	if (Agent->GetRotation().z > angle + angleTolerance || Agent->GetRotation().z < angle - angleTolerance)
	{
		if (angle < Agent->GetRotation().z)
		{
			Agent->SetAngularSpeed(vec3(0.0f, 0.0f, -25.0f));
		}
		else
		{
			Agent->SetAngularSpeed(vec3(0.0f, 0.0f, +25.0f));
		}
		return;
	}

	if (distanceToTarget > 25.0f)
	{
		Agent->SetThrustMode(1.0f);
		//Agent->SetLinearSpeed(direction * 1.0f * distanceToTarget);
		return;
	}
	else if (distanceToTarget < 25.0f)
	{
		Agent->SetThrustMode(0.0f);
		return;
	}

	if (this->shotCooldown > 0.0f)
		this->shotCooldown -= t;
	else
	{
		Agent->ShotAt(targetPos);
		this->shotCooldown = 2.0f;
	}

}