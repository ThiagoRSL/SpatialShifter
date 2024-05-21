#pragma once

#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <iostream>
#include <vector>

#include "TextureManager.h"
#include "GLUtils.h"
#include "../entities/Ship.h"
#include "../colliders/PolyCollider.h"

class ShipBuilder
{	

private:
	static ShipBuilder* shipBuilderInstance;
	GLFWwindow* window = nullptr;

	ShipBuilder();
	std::map<ShipTypeId, const char*> ShipDict;
	std::map<ShipTypeId, const char*> ColliderDict;
	std::map<ProjectileTypeID, const char*> ProjectileDict;
	std::map<ProjectileTypeID, const char*> ProjectileColliderDict;
	void InitDict() 
	{ 
		ShipDict = std::map<ShipTypeId, const char*>(); 
		ColliderDict = std::map<ShipTypeId, const char*>();
		ProjectileDict = std::map<ProjectileTypeID, const char*>();
		ProjectileColliderDict = std::map<ProjectileTypeID, const char*>();
	}


public:
	static ShipBuilder* GetInstance()
	{
		if (shipBuilderInstance == nullptr)
		{
			shipBuilderInstance = new ShipBuilder();
		}
		return shipBuilderInstance;
	};


public:
	const char* GetShipModelPath(ShipTypeId shipTypeId);
	const char* GetColliderModelPath(ShipTypeId shipTypeId);
	const char* GetProjectileModelPath(ProjectileTypeID projectileTypeId);
	const char* GetProjectileColliderPath(ProjectileTypeID projectileTypeId);
	
	Ship BuildShip(ShipTypeId SHIP_TYPE_ID);
};