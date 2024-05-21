
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
	inertiaMoment = 1.0f;
}


void Entity::AddInstantaneousForce(Force f)
{
	instantaneousForces.push_back(f);
}
