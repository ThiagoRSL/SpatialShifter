#pragma once

#include <GL/glew.h>

#include <GLFW\glfw3.h>
#include <vector>

#include "../Entities/Entity.h"
#include "../utils/GlobalVars.h"
#include "../models/ShipModel.h"
#include "../colliders/PolyCollider.h"
#include "../scripts/skills/SkillAbstract.h"

#define WINDOW_WIDTH	1000.0
#define WINDOW_HEIGHT	1000.0

class Ship;

enum class ShipAutonomyState {
	WANDER,
	SEARCH,
	SEEK,
	SLAM,
	SHOOT,
};

class AutonomyShipModule
{

private:
	bool starting;
	Ship* Agent;
	float awareRadius;
	float shotCooldown;
	ShipAutonomyState State;

	vec3* EnemyPosition;
	vec2 TargetPosition;
	vec2 WanderToPosition;

public:
	AutonomyShipModule() { ; }
	AutonomyShipModule(Ship* agent, float aware_radius, vec3* enemyPosition);

	// mesh virtual functions
	void Init(ShipTypeId shipTypeId);
	void Update(double t);
	void Destroy();

	void Wandering(double t);
	void Shooting(double t);
public:

};


/*
Uma nave espacial leve pesa entre 40 e 100 toneladas e requer de dez a trinta tripulantes.
Uma nave espacial média pesa entre 100 e 1000 toneladas e requer de vinte a quinhentos tripulantes.
Uma nave espacial pesada pesa mais de 1000 toneladas e requer mais de quinhentos tripulantes para operar.
*/