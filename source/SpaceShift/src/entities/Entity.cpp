
#include "Entity.h"
using namespace std;

Entity::Entity()
{
	forces = std::vector<Force*>();
	this->Init(); 
}

Entity::Entity(GLFWwindow* window, int size)
{
	size = size;
	forces = std::vector<Force*>();
	this->Init(); 
}

void Entity::RemoveForce(Force* f)
{
	// Encontra a força e a remove, depois retorna.
	for (int i = 0; i < forces.size(); i++)
	{
		if (forces.at(i) == f)
		{
			forces.erase(forces.begin() + i);
			return;
		}
	}
	return;
}

void Entity::UpdateForces(double t)
{
	// Dado por newtons.
	forcesSum = vec3(0.0f, 0.0f, 0.0f);
	torque = vec3(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < forces.size(); i++)
	{
		vec3 target = forces.at(i)->GetTargetPoint();
		vec3 f = forces.at(i)->GetVec();
		if (target == vec3(0.0f))
		{
			//Calcula a força das somas para o deslocamento vetorial (Translação)
			forcesSum += vec3(f.x, f.y, f.z);
		}
		else
		{
			//Calcula o torque das forças para a rotação do objeto.
			torque += glm::cross(target, f);
		}

	}
	//Calcula as forças instantâneas depois as limpa.
	for (int i = 0; i < instantaneousForces.size(); i++)
	{
		//Calcula a força das somas para o deslocamento vetorial (Translação)
		vec3 f = instantaneousForces.at(i).GetVec();
		vec3 target = instantaneousForces.at(i).GetTargetPoint();
		if (target == vec3(0.0f))
		{
			//Calcula a força das somas para o deslocamento vetorial (Translação)
			forcesSum += vec3(f.x, f.y, f.z);
		}
		else
		{
			//Calcula o torque das forças para a rotação do objeto.
			torque += glm::cross(target, f);
		}
	}
	instantaneousForces.clear();

}

void Entity::UpdateAcceleration(double t)
{
	// Dado por metros por segundo ao quadrado.
	acceleration = forcesSum / vec3(mass);
	angularAcceleration = torque / vec3(inertiaMoment);
}

void Entity::UpdateSpeed(double t)
{
	// Dado por metros por segundo.
	linearSpeed += acceleration * vec3(t);
	angularSpeed += angularAcceleration * vec3(t);
}

void Entity::UpdatePosition(double t)
{
	position += linearSpeed * vec3(t);
	rotation += angularSpeed * vec3(t);
	
	// Normalizando para o intervalo [0, 360)
	rotation.x = fmod(rotation.x, 360.0f);
	rotation.y = fmod(rotation.y, 360.0f);
	rotation.z = fmod(rotation.z, 360.0f);

	// Garantir que o valor de rotação seja sempre positivo
	if (rotation.x < 0) rotation.x += 360.0f;
	if (rotation.y < 0) rotation.y += 360.0f;
	if (rotation.z < 0) rotation.z += 360.0f;
}



void Entity::Update(double t)
{
	this->UpdateForces(t);
	this->UpdateAcceleration(t);
	this->UpdateSpeed(t);
	this->UpdatePosition(t);

	//a é a aceleração linear do ponto
	//r é a distância do ponto ao eixo de rotação
	//α é a aceleração angular do objeto
}
void Entity::Init()
{
	isDestroyed = false;
	inertiaMoment = 1.0f;
	position = vec3(0.0f, 0.0f, 1.0f);
	rotation = vec3(0.0f, 0.0f, 0.0f);
	direction = vec3(1.0f, 0.0f, 0.0f);
}


void Entity::AddInstantaneousForce(Force f)
{
	instantaneousForces.push_back(f);
}

void Entity::ClearInstantaneousForce()
{
	instantaneousForces.clear();
}

void Entity::RotateDirection()
{
	direction.x = 1.0 * glm::cos(glm::radians(this->rotation.z + 90.0f));
	direction.y = 1.0 * glm::sin(glm::radians(this->rotation.z + 90.0f));
}




vec2 Entity::ThrustToGo(vec2 goalPosition, double desiredTime, float SpeedLimit)
{
	float mass = this->GetMass();
	vec2 currentPosition = vec2(this->GetPosition());
	vec2 currentSpeed = vec2(this->GetLinearSpeed());
	vec2 currentAcceleration = vec2(this->GetLinearAcceleration());

	vec2 time = vec2(desiredTime);
	vec2 distance = goalPosition - currentPosition;

	vec2 targetSpeed = distance / time;
	vec2 targetSpeedDirection = MathUtils::Normalize(targetSpeed);

	/*if (MathUtils::Norm(targetSpeed) > SpeedLimit) // Tratamento para não estourar o limite de velocidade recebido.
	{
		targetSpeed.x = (targetSpeedDirection.x * SpeedLimit);
		targetSpeed.y = (targetSpeedDirection.y * SpeedLimit);
	}*/

	vec2 deltaSpeed = (targetSpeed - currentSpeed);

	vec2 targetAcceleration = (deltaSpeed / time);
	vec2 targetThrust = mass * targetAcceleration;

	return targetThrust;
}

double Entity::TimeToReach(vec2 goalPosition)
{
	float mass = this->GetMass();
	vec2 currentPosition = vec2(this->GetPosition());
	vec2 currentSpeed = vec2(this->GetLinearSpeed());
	vec2 distanceToGoal = goalPosition - currentPosition;

	// Se a velocidade do agente for zero e a distância for positiva, o agente nunca atingirá o objetivo
	if (MathUtils::Norm(currentSpeed) == 0.0f)
		return std::numeric_limits<double>::infinity(); // Nunca atingirá

	// Verifica se a velocidade está apontando para longe do objetivo (produto escalar negativo)
	float dotProduct = MathUtils::DotProduct(currentSpeed, distanceToGoal);

	if (dotProduct <= 0.0f)
		return std::numeric_limits<double>::infinity(); // Nunca atingirá

	double t = MathUtils::Norm(distanceToGoal) / MathUtils::Norm(currentSpeed);
	return t;
}