
#include "CollisionManager.h"
using namespace std;

class GlobalVars;

CollisionManager* CollisionManager::collisionManagerInstance = nullptr;


CollisionManager::CollisionManager()
{

}

void CollisionManager::VerifyCollisions()
{
	std::vector<AbstractCollider*> nearColliders;
	std::vector<AbstractCollider*> nearProjectiles;
	if (GlobalVars::DEBUG_MODE)
	{
		nearColliders.clear();
		nearColliders = AllCollidersInCollidingRadius(CameraManager::GetInstance()->WorldPivot(), GlobalVars::STANDARD_COLLIDING_RADIUS);
		for (int i = 0; i < nearColliders.size(); i++)
		{
			nearColliders.at(i)->SetDebugColor(vec4(1.0f, 0.0f, 0.0f, 1.0f));
		}
	}

	//#pragma omp parallel for 
	for (int i = 0; i < ColliderList.size(); i++)
	{
		//TODO: MELHORAR SELEÇÂO DE COLISÂO
		//Aplicar algum algoritmo para garantir calculo só daquilo que é necessário,
		//Por exemplo o tal do spatialhash ou algum outro que faça calculos extremamente simples 
		//Para verificar uma possível colisão e depois calculos mais específicos.
		//Podendo verificar a um nivel hierarquico por exemplo, utilizando uma bounding-box.
		//O for abaixo então só verificaria objetos que de fato podem estar colidindo entre si no momento.

		// Linear Speed computar a verificação de colisão de maneira PREDITIVA, não de maneira reparativa como está sendo agora.
		// Assim é possível ajustar o número de checagens através da velocidade do objeto
		// ColliderList.at(i)->GetEntity()->GetSpeed(); 

		nearColliders.clear();
		nearColliders = CollisionCheck(i);

		for (int j = 0; j < nearColliders.size(); j++)
		{

			//Pula colisão caso ela não ocorra por serem elementos da mesma equipe
			//TODO: Indexar elementos préviamente por facção.
			if (!GlobalVars::SAME_FACTION_COLLIDE)
			{
				if (ColliderList.at(i)->GetEntity()->GetFaction() == nearColliders.at(j)->GetEntity()->GetFaction())
				{
					continue;
				}
			}
            if (ColliderList.at(i) != nearColliders.at(j) && ColliderList.at(i)->isColliding((*nearColliders.at(j))))
            {
				if (GlobalVars::DEBUG_MODE) 
				{
					ColliderList.at(i)->SetDebugColor(vec4(0.0f, 1.0f, 0.0f, 1.0f));
					nearColliders.at(j)->SetDebugColor(vec4(0.0f, 1.0f, 0.0f, 1.0f));
				}

				vec3 collisionPoint = ColliderList.at(i)->CollidingAt(*nearColliders.at(j));
				collisionPoint = vec3((CameraManager::GetInstance()->MVP() * vec4(collisionPoint, 1.0f)));

				ColliderList.at(i)->GetEntity()->CollideAtProjection(
						*nearColliders.at(j)->GetEntity(),
						collisionPoint
					);
            }
		}

		nearProjectiles.clear();
		//TROACAR A LOGICA PRA USAR O RADIUS DOS OUTROS COLLIDERS E NAO DO PROJECTILE
		nearProjectiles = AllProjectileCollidersInCollidingRadius(ColliderList.at(i)->GetEntity()->GetPosition(), ColliderList.at(i)->GetCollidingRadius());

		for (int j = 0; j < nearProjectiles.size(); j++)
		{

			//Pula colisão caso ela não ocorra por serem elementos da mesma equipe
			//TODO: Indexar elementos préviamente por facção.

			if (!GlobalVars::SAME_FACTION_COLLIDE)
			{
				if (ColliderList.at(i)->GetEntity()->GetFaction()
					== nearProjectiles.at(j)->GetEntity()->GetFaction())
				{
					continue;
				}
			}

			if (ColliderList.at(i)->isColliding((*nearProjectiles.at(j))))
			{
				//printf("\nTirambaço colidiu!");
				if (GlobalVars::DEBUG_MODE)
				{
					//ColliderList.at(i)->SetDebugColor(vec4(0.0f, 1.0f, 0.0f, 1.0f));
					nearProjectiles.at(j)->SetDebugColor(vec4(0.0f, 1.0f, 0.0f, 1.0f));
				}

				vec3 collisionPoint = ColliderList.at(i)->CollidingAt(*nearProjectiles.at(j));
				collisionPoint = vec3((CameraManager::GetInstance()->MVP() * vec4(collisionPoint, 1.0f)));

				ColliderList.at(i)->GetEntity()->CollideAtProjection(
					*nearProjectiles.at(j)->GetEntity(),
					collisionPoint
				);
				break;
			}
		}
	}
}

//Pega todos os colliders que ainda não verificaram colisão com o collider atual.
//Verifica se há possibilidade de ter colisão entre ambos (checa a soma dos raios)
std::vector<AbstractCollider*> CollisionManager::CollisionCheck(int colliderIndex)
{
	glm::vec3 position1 = ColliderList.at(colliderIndex)->GetEntity()->GetPosition();
	float checkRadius1 = ColliderList.at(colliderIndex)->GetCollidingRadius();

	std::vector<AbstractCollider*> CollidersInRadius = std::vector<AbstractCollider*>();
	for (int i = colliderIndex+1; i < ColliderList.size(); i++)
	{
		glm::vec3 position2 = ColliderList.at(i)->GetEntity()->GetPosition();
		float checkRadius2 = ColliderList.at(i)->GetCollidingRadius();
		float distance = MathUtils::Norm(position1 - position2);
		if (distance < checkRadius1 + checkRadius2)
			CollidersInRadius.push_back(ColliderList.at(i));
	}
	return CollidersInRadius;
}

std::vector<AbstractCollider*> CollisionManager::AllCollidersInCollidingRadius(glm::vec3 point, float collidingRadius)
{
	std::vector<AbstractCollider*> CollidersInRadius = std::vector<AbstractCollider*>();

	for (int i = 0; i < ColliderList.size(); i++)
	{
		vec3 entityPos = ColliderList.at(i)->GetEntity()->GetPosition();
		vec3 distance = point - entityPos;
		if (sqrt(pow(distance.x, 2) + pow(distance.y, 2) + pow(distance.z, 2)) 
			< collidingRadius + ColliderList.at(i)->GetCollidingRadius())
			CollidersInRadius.push_back(ColliderList.at(i));
	}

	return CollidersInRadius;
}

std::vector<AbstractCollider*> CollisionManager::AllProjectileCollidersInCollidingRadius(glm::vec3 point, float collidingRadius)
{
	std::vector<AbstractCollider*> CollidersInRadius = std::vector<AbstractCollider*>();

	for (int i = 0; i < ProjectileCollidersList.size(); i++)
	{
		vec3 entityPos = ProjectileCollidersList.at(i)->GetEntity()->GetPosition();
		vec3 distance = point - entityPos;
		if (sqrt(pow(distance.x, 2) + pow(distance.y, 2) + pow(distance.z, 2))
			< collidingRadius + ProjectileCollidersList.at(i)->GetCollidingRadius())
		{
			CollidersInRadius.push_back(ProjectileCollidersList.at(i));
		}
	}

	return CollidersInRadius;
}

std::vector<Entity*> CollisionManager::AllEntitiesInRadius(glm::vec3 point, float radius)
{
	std::vector<Entity*> EntitiesInRadius = std::vector<Entity*>();

	for(int i = 0; i < ColliderList.size(); i++)
	{
		vec3 entityPos = ColliderList.at(i)->GetEntity()->GetPosition();
		vec3 distance = point - entityPos;
		if (sqrt(pow(distance.x,2) + pow(distance.y, 2) + pow(distance.z, 2)) < radius)
			EntitiesInRadius.push_back(ColliderList.at(i)->GetEntity());
	}

	for (int i = 0; i < ProjectileCollidersList.size(); i++)
	{
		vec3 entityPos = ProjectileCollidersList.at(i)->GetEntity()->GetPosition();
		vec3 distance = point - entityPos;
		if (sqrt(pow(distance.x, 2) + pow(distance.y, 2) + pow(distance.z, 2)) < radius)
			EntitiesInRadius.push_back(ProjectileCollidersList.at(i)->GetEntity());
	}

	return EntitiesInRadius;
}

/*
void CollisionManager::ChangeFactionIndex(AbstractCollider* collider, Faction oldFaction, Faction newFaction);

std::vector<AbstractCollider*>* CollisionManager::SelectColliderListIndexed(Faction f)
{
	auto it = ColliderListTeamIndexed.find(f);

	if (it != ColliderListTeamIndexed.end()) {
		return &(it->second);
	}
	else {
		printf("\n !!!!! Não existe a facção buscada. !!!!!");
		return nullptr;
	}
	
}
*/


