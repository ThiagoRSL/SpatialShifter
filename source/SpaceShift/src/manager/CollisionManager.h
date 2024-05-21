#pragma once

#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <iostream>
#include <vector>

#include "TextureManager.h"
#include "GLUtils.h"
#include "../entities/Entity.h"
#include "../colliders/PolyCollider.h"
#include "../colliders/PointCollider.h"
#include "../utils/GlobalVars.h"

class CollisionManager
{

private:
	static CollisionManager* collisionManagerInstance;
	GLFWwindow* window = nullptr;

	CollisionManager();
	std::vector<AbstractCollider*> ProjectileCollidersList;

	std::vector<AbstractCollider*> ColliderList;
	std::map<Faction, std::vector<AbstractCollider*>> ColliderListTeamIndexed;

	//std::vector<AbstractCollider*>* SelectColliderListIndexed(Faction f);

public:

	void InitColliderList() { ColliderList = std::vector<AbstractCollider*>(); ProjectileCollidersList = std::vector<AbstractCollider*>(); }
	void AddProjectileCollider(AbstractCollider* projectileCollider) { ProjectileCollidersList.push_back(projectileCollider); }
	void AddCollider(AbstractCollider* collider)	{ ColliderList.push_back(collider); }
	
	void RemoveProjectileCollider(AbstractCollider* collider) { ProjectileCollidersList.erase(std::remove(ProjectileCollidersList.begin(), ProjectileCollidersList.end(), collider), ProjectileCollidersList.end()); }
	void RemoveCollider(PolyCollider* collider) { printf("\n Collider elements: %i", ColliderList.size()); ColliderList.erase(std::remove(ColliderList.begin(), ColliderList.end(), collider), ColliderList.end()); }
	
	void VerifyCollisions();

	std::vector<AbstractCollider*> CollisionCheck(int colliderIndex);
	std::vector<Entity*> AllEntitiesInRadius(glm::vec3 point, float radius);
	std::vector<AbstractCollider*> AllCollidersInCollidingRadius(glm::vec3 point, float collidingRadius);
	std::vector<AbstractCollider*> AllProjectileCollidersInCollidingRadius(glm::vec3 point, float collidingRadius);
	//void ChangeFactionIndex(AbstractCollider* collider, Faction oldFaction, Faction newFaction);
	//void AddCollider(AbstractCollider* collider) { RenderList.push_back(entity); }
	
public:

	static CollisionManager* GetInstance()
	{
		if (collisionManagerInstance == nullptr)
		{
			collisionManagerInstance = new CollisionManager();
			collisionManagerInstance->InitColliderList();
		}
		return collisionManagerInstance;
	};
};