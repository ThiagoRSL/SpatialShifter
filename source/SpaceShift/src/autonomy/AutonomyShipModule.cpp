#include "AutonomyShipModule.h"
#include "../Entities/Ship.h"
#include "./Task/Task.h"

AutonomyShipModule::AutonomyShipModule(Ship* agent, float awareRadius, vec3* enemyPosition)
{
	CurrentTask = nullptr;
	starting = false;
	Agent = agent;
	this->CurrentBehavior = AutonomyBehavior::NONE;
	this->awareRadius = awareRadius;
	//this->shootingDistance = 100.0f;
	this->autonomyShotCooldown = 2.0f;

	//State = ShipAutonomyState::WANDER;
	//WanderToPosition = vec2(Agent->GetPosition());
	EnemyPosition = enemyPosition;
}

void AutonomyShipModule::Update(double t)
{
	if (CurrentTask == nullptr)
	{
		if (Tasks.empty())
			return;

		Task* t = PeekTask();
		if (t->CheckRequirements()) CurrentTask = FetchTask();
		if (CurrentTask != nullptr)
			CurrentTask->State = TaskState::STARTING;		
	}

	if (CurrentTask == nullptr)
		return;

	switch (CurrentTask->State)
	{
		case TaskState::STARTING:
			CurrentTask->Start();
			break;
		case TaskState::EXECUTING:
			CurrentTask->Run(t);
			break;
		case TaskState::ENDED:
			CurrentTask = nullptr;
			break;
	}
	


	/*if (MathUtils::Norm(*EnemyPosition - Agent->GetPosition()) < this->awareRadius)
	{
		//Agent->SetAngularSpeed(vec3(0.0f));
		//Agent->SetLinearSpeed(vec3(0.0f));
		//this->State = ShipAutonomyState::COMBAT;
	}*/


	/*switch (this->CurrentBehavior) {
		case AutonomyBehavior::WANDER:
			//Wandering(t);
			break;
		case AutonomyBehavior::COMBAT:
			//Combating(t);
			break;
		case AutonomyBehavior::SEEK:
			Seeking(t);
			break;
		case AutonomyBehavior::STABILIZE:
			if(Stabilize()) this->CurrentBehavior = AutonomyBehavior::NONE;
			break;
		case AutonomyBehavior::NONE:
			if(!this->IsStabilized()) this->CurrentBehavior = AutonomyBehavior::STABILIZE;
			break;
	}*/
}

void AutonomyShipModule::SeekTo(vec3 targetLocation)
{
	this->CurrentBehavior = AutonomyBehavior::SEEK;
	this->SeekPosition = vec2(targetLocation);	
}

bool AutonomyShipModule::Stabilize()
{
	vec2 agentPos = vec2(Agent->GetPosition());
	float distanceToTarget = MathUtils::Norm(SeekPosition - agentPos);
	glm::vec3 CurrentSpeed = Agent->GetLinearSpeed();
	float CurrentSpeedNorm = MathUtils::Norm(vec2(CurrentSpeed));

	if (CurrentSpeedNorm <= StabilizedSpeedTolerance)
	{
		Agent->SetThrustByMax(0.0f);
		return true;
	}

	// Verifica a Direção para a qual está se deslocando atualmente.

	glm::vec3 speedDirection = glm::normalize(CurrentSpeed);
	// TODO: Desconsidera a velocidade no eixo Z
	float angle = -glm::degrees(atan2(-speedDirection.x, -speedDirection.y)); // -x, x belongs to [-180, 180]

	// Essa função rotaciona a nave até atingir o angulo específico, esse angulo agora é o angulo oposto da velocidade
	// Ou seja, ele visa desacelerar a nave.
	if (!this->RotateToAngle(angle, 0.25f))
		return false;

	float DeltaThrust = max(0.0f, (CurrentSpeedNorm / SpeedLimit));
	Agent->SetThrustByMax(DeltaThrust);

	if (CurrentSpeedNorm <= StabilizedSpeedTolerance)
	{
		Agent->SetThrustByMax(0.0f);
		Agent->SetLinearSpeed(vec3(0.0f)); // -> Só ocorre após ter atingido a Tolerância (atualmente 0.1)
		return true;
	}

	return false;
}

bool AutonomyShipModule::RotateToAngle(float angle, float angleTolerance)
{	
	vec3 AgentRotation = Agent->GetRotation();
	// Ajuste do direcionamento do Agente
	if (!MathUtils::AngleInRange(AgentRotation.z, angle, angleTolerance))
	{
		Agent->SetThrustByMax(0.0f);
		// Calculando a diferença de ângulo.
		float diff = angle - Agent->GetRotation().z;

		// Normalizando a diferença para o intervalo [-180°, 180°].
		if (diff > 180.0f) diff -= 360.0f;   // Ajuste se for maior que 180
		else if (diff < -180.0f) diff += 360.0f; // Ajuste se for menor que -180

		// Se diff é positivo, o agente precisa rotacionar no sentido anti-horário (esquerda).
		// Se diff é negativo, o agente precisa rotacionar no sentido horário (direita).
		float deltaAngle = 10.0f + 2 * fabs(diff);

		// Agora, determinamos a direção com base em "diff".
		if (diff > 0.0f)  // Se diff é positivo, gira no sentido anti-horário
		{
			Agent->SetAngularSpeed(vec3(0.0f, 0.0f, deltaAngle));
		}
		else if (diff < 0.0f)  // Se diff é negativo, gira no sentido horário
		{
			Agent->SetAngularSpeed(vec3(0.0f, 0.0f, -deltaAngle));
		}
		return false;
	}
	else
	{
		Agent->SetRotation(vec3(AgentRotation.x, AgentRotation.y, angle));
		Agent->SetAngularSpeed(vec3(0.0f));
	}
	return true;
}

bool AutonomyShipModule::MovingTowards(float angle, float angleTolerance)
{
	glm::vec3 CurrentSpeed = Agent->GetLinearSpeed();
	float CurrentSpeedNorm = MathUtils::Norm(CurrentSpeed);
	if(angle)
		return true;
	else
		return false;
}

bool AutonomyShipModule::Seeking(double t)
{
	vec2 agentPos = vec2(Agent->GetPosition());
	float distanceToTarget = MathUtils::Norm(SeekPosition - agentPos);
	glm::vec3 CurrentSpeed = Agent->GetLinearSpeed();
	float CurrentSpeedNorm = MathUtils::Norm(CurrentSpeed);
	float angleSpeed = -glm::degrees(atan2(CurrentSpeed.x, CurrentSpeed.y)); // -x, x belongs to [-180, 180]

	//Calcula a direção que quer ir
	glm::vec3 direction = glm::normalize(vec3(SeekPosition.x - agentPos.x, SeekPosition.y - agentPos.y, 0.0f));
	float angle = -glm::degrees(atan2(direction.x, direction.y)); // -x, x belongs to [-180, 180]
	float angleInverse = -glm::degrees(atan2(-direction.x, -direction.y)); // -x, x belongs to [-180, 180]
	this->AimedSpeed = glm::normalize(direction) * SpeedLimit;

	// Verifica se atingiu o objetivo.
	if (distanceToTarget < 5.0f)
	{
		Agent->SetThrustByMax(0.0f);
		Agent->SetLinearSpeed(vec3(0.0f));
		this->CurrentBehavior = AutonomyBehavior::NONE;
		return true;
	}

	// Verifica se está estabilizada
	if (!MathUtils::AngleInRange(angle, angleSpeed, 1.0f))	// !this->IsStabilized() && 
	{
		if (!this->Stabilize())
			return false;
	}

	//Calcula a projeção em X iterações futuras
	double projectionIteration = 10.0;
	vec3 positionProjection = Agent->GetPosition() + Agent->GetLinearSpeed() * vec3(projectionIteration * t);
	float distanceProjectionToTarget = MathUtils::Norm(SeekPosition - vec2(positionProjection));
	float remainingTravelTime = distanceToTarget / CurrentSpeedNorm;


	// Ajusta o Angulo em direção do Objetivo
	if (!this->RotateToAngle(angle, 1.0f))
		return false;

	//Iniciar a propulsão em função da distância até o alvo.
	float DeltaThrust = max(0.0f, 1.0f - (CurrentSpeedNorm / SpeedLimit));
	Agent->SetThrustByMax(DeltaThrust);
	return false;
}

bool AutonomyShipModule::Wandering(double t)
{
	return false;
	// Wander and Seek

	// Se tiver próximo vaga para outra área.
	/*if (MathUtils::Norm(SeekPosition - agentPos) < 5.0f)
	{
		float rx = 10.0f - static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 20));
		float ry = 10.0f - static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 20));
		SeekPosition = vec2(agentPos.x + rx, agentPos.y + ry);
	}*/
}


bool AutonomyShipModule::Combating(double t)
{
	return false;
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
		return false;
	}

	if (distanceToTarget > 25.0f)
	{
		//Agent->SetThrustMode(distanceToTarget/awareRadius);
		Agent->SetLinearSpeed(direction * 1.0f * distanceToTarget);
		//return;
	}
	else
	{
		//Agent->SetThrustMode(0.0f);
		Agent->SetLinearSpeed(vec3(0.0f));
		//return;
	}

	if (this->autonomyShotCooldown > 0.0f)
		this->autonomyShotCooldown -= t;
	else
	{
		Agent->ShotAt(targetPos);
		this->autonomyShotCooldown = 2.0f;
	}

	return false;
}



void AutonomyShipModule::AddTaskStart(Task* t) { Tasks.push_front(t); t->AddedQeue(this); }
void AutonomyShipModule::AddTaskEnd(Task* t) { Tasks.push_back(t); t->AddedQeue(this); }

bool AutonomyShipModule::IsStabilized() { return MathUtils::Norm(Agent->GetLinearSpeed()) < StabilizedSpeedTolerance; }