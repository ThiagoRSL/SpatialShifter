#pragma once

#include <GL/glew.h>

#include <GLFW\glfw3.h>
#include <vector>

#include "Entity.h"
#include "../utils/GlobalVars.h"
#include "../models/ShipModel.h"
#include "../colliders/PolyCollider.h"
#include "../scripts/skills/SkillAbstract.h"
#include "projectiles/projectile.h"
#include "../autonomy/AutonomyShipModule.h"

#define WINDOW_WIDTH	1000.0
#define WINDOW_HEIGHT	1000.0

class StageManager;
class ShipBuilder;

class Ship : public Entity
{

private:
	ShipTypeId ShipType;
	bool wireframe = false;
	AutonomyShipModule* Autonomy;
	ShipState shipState; 
	bool isAutonomous;

	float shotCooldown;
	float shotCooldownTime;

	vec3 direction;
	Force thrustForce;
	Force rotationWheelForce;
	Force rotationWheelForceCounter;
	Force rotationWheelXForce;
	Force rotationWheelXForceCounter;

	float speedMax;
	float rotateDelta;

	float thrustMultiplier;
	float thrustRelativeIntensity;
	float thrustIntensity;
	float thrustIntensityMax;

	double destroyedTime;

	ShipModel Model;
	PolyCollider Collider;

	std::vector<SkillAbstract*> ActiveSkills;
	std::vector<SkillAbstract*> SkillsToEnd;

	void UpdateDestroyedTime(double deltaTime);
public:
	
	Ship(ShipTypeId shipTypeId);

	// mesh virtual functions
	void Init();
	void Update(double t);
	void Render() 
	{ 
		this->Model.Render(); 

		if (GlobalVars::DEBUG_MODE)
		{ 
			this->Collider.Render();
		}
	}//Extrair para entity
	void Destroy();

	void UpdateLight(double deltaTime);
	void UpdateMass();
	void UpdateSpeed(double deltaTime);
	void UpdateThrust(double deltaTime);
	void UpdatePosition(double deltaTime);

	void AdjustRotationZ(float WheelForceVal);
	void AdjustRotationX(float WheelForceVal);
	void RotateDirection();
	void UpdateRotation();

	void CalculateShipInertia(const std::vector<glm::vec3>& vertexes);
	void CalculateThrust(double deltaTime);

	void ShotAt(vec3 shotTarget);
	void UseSkill(vec3 shotTarget);
	void SetThrustMode(float value);
	DamageCallback ReceiveDamage(float damage, DamageType damageType);

	void CollideWith(Entity& otherEntity, glm::vec3 collisionPoint);
	void CollideWithShip(Ship& otherShip, glm::vec3 collisionPoint);
	void CollideWithProjectile(Projectile& otherProjectile, glm::vec3 collisionPoint);
	void CollideAtProjection(Entity& otherEntity, glm::vec3 collisionPoint);


public:

	mat4 GetRotationMatrix() { return this->Model.GetRotationMatrix(); }
	mat4 GetScaleMatrix() { return this->Model.GetScaleMatrix(); }
	vec4 GetColliderDebugColor() { return Collider.GetDebugColor(); }
	double GetMomentum() { return this->mass * MathUtils::Norm(this->linearSpeed); }
	float GetDestroyedTime() { return this->destroyedTime; }
	ShipState GetShipState() { return this->shipState; }
	ShipTypeId GetShipType() { return this->ShipType; }
	float GetThrustIntensity() { return thrustIntensity; }
	float GetThrustIntensityMax() { return thrustIntensityMax; }

	// Setters
	void SetScale(vec3 scale) { Model.SetScale(scale); }
	void SetShipColor(vec4 shipColor) { Model.SetShipModelColor(shipColor); }
	void SetCollidingRaidius(float r) { return this->Collider.SetCollidingRadius(r); }
	void SetAutonomous(AutonomyShipModule* ASM);

	void SetThrustIntensityMax(float thrustMax) { this->thrustIntensityMax = thrustMax; }
};


/*
Uma nave espacial leve pesa entre 40 e 100 toneladas e requer de dez a trinta tripulantes.
Uma nave espacial média pesa entre 100 e 1000 toneladas e requer de vinte a quinhentos tripulantes.
Uma nave espacial pesada pesa mais de 1000 toneladas e requer mais de quinhentos tripulantes para operar.
*/