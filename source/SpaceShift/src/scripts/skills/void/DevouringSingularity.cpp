
#include "DevouringSingularity.h"
using namespace std;

DevouringSingularity::DevouringSingularity(Entity* sourceEntity, vec3 targetpoint, float mass, float radius, float dps, double duration)
{
	this->source = sourceEntity;
	this->singularityColor = GlobalColors::DEVOURING_SINGULARITY_COLOR;
	this->SingularityMass = mass;
	this->AttractingRadius = radius;
	this->AttractingCenter = targetpoint;
	this->DamagePerSecond = dps;
	this->timeToLive = duration;

	this->Model = DevouringSingularityModel(this);
	this->Model.Init();
}

void DevouringSingularity::Init()
{
	return;
}

void DevouringSingularity::Update(double t)
{
	if (this->State == SkillState::ENDED) return;

	UpdateAttractingEntities(t);
	this->timeToLive -= t;
	if (this->timeToLive <= 0.0) this->End();
	Render();
}


void DevouringSingularity::End()
{
	this->State = SkillState::ENDED;
	return;
}

//Fazer a singularidade ter a massa aumentada em função das naves destruídas e engolidas por ela.
//Extendendo sua duração e aumentando seu raio de ação e dano por segundo.
//Atualiza a lista a cada interação (identificdar novas entidades que estão próximas.)
void DevouringSingularity::UpdateAttractingEntities(double deltaTime)
{
	if(this->State == SkillState::ENDED) return;

	std::vector<Entity*> targets = CollisionManager::GetInstance()->AllEntitiesInRadius(AttractingCenter, AttractingRadius);
	// Atracting é registro do dicionário

	for (const auto& attracting : targets)
	{
		float distance = MathUtils::Norm(attracting->GetPosition() - AttractingCenter);
		if (distance < AttractingRadius)
		{

			vec3 direction = AttractingCenter - attracting->GetPosition();
			direction = glm::normalize(direction);
			direction.z = 0.f;	//Não deixa puxar pra cima

			float forceIntensity = ((float)attracting->GetMass() * SingularityMass) / ((0.5) + (distance * distance));
			attracting->AddInstantaneousForce(Force(direction * forceIntensity));
			attracting->ReceiveDamage(DamagePerSecond * deltaTime, DamageType::VOID_DAMAGE);
		}
		// o que fazer caso o elemento estiver no dicionário e não pertencer ao raio? (MARCAR PARA EXCLUSÂO)
	}



}



