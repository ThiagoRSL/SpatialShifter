
#include "DevouringSingularity.h"
using namespace std;

DevouringSingularity::DevouringSingularity(Entity* sourceEntity, vec3 targetpoint, float mass, float dps, double duration)
{
	this->source = sourceEntity;
	this->singularityColor = GlobalColors::DEVOURING_SINGULARITY_COLOR;
	this->SingularityMass = mass;
	this->AttractingRadius = mass * 0.001;
	this->AttractingCenter = targetpoint;
	this->BaseDamagePerSecond = dps;
	this->timeToLive = mass * 0.0002;

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
		float distance = MathUtils::Norm((attracting->GetPosition() - CameraManager::GetInstance()->WorldPivot()) - (AttractingCenter - CameraManager::GetInstance()->WorldPivot()));
		if (distance < AttractingRadius)
		{

			vec3 direction = (AttractingCenter - CameraManager::GetInstance()->WorldPivot()) - (attracting->GetPosition() - CameraManager::GetInstance()->WorldPivot());
			direction = glm::normalize(direction);
			direction.z = 0.f;	//Não deixa puxar pra cima

			float forceIntensity = ((float)attracting->GetMass() * SingularityMass) / (10 * (distance * distance));
			attracting->AddInstantaneousForce(Force(direction * forceIntensity));

			float dmg;

			if (distance < 2.5)
			{
				dmg = 1000000;

				attracting->SetLinearSpeed(vec3(0.0, 0.0, 0.0));
				attracting->ClearInstantaneousForce();
				attracting->SetAngularSpeed(vec3(MathUtils::RandomBetween(10.0, 100.0), MathUtils::RandomBetween(10.0, 100.0), MathUtils::RandomBetween(10.0, 100.0)));
			}
			else
			{
				dmg = BaseDamagePerSecond * deltaTime;
			}
			
			if (attracting->ReceiveDamage(dmg, DamageType::VOID_DAMAGE) == DamageCallback::DESTROYED)
			{
				this->SingularityMass += attracting->GetMass();
				this->AttractingRadius += attracting->GetMass() * 0.01;
				this->timeToLive += attracting->GetMass() * 0.002;
			}

		}
		// o que fazer caso o elemento estiver no dicionário e não pertencer ao raio? (MARCAR PARA EXCLUSÂO)
	}
}



