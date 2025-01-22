#pragma once

#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <vector>

#include "../common/Shader.h"
#include "../physics/Force.h"
#include "../physics/SpacePhysics.h"
#include <iostream>
#include "TextureManager.h"
#include "../common/ObjectLoader.h"
#include "../manager/CameraManager.h"
#include "../Utils/MathUtils.h"
#include "../Utils/GlobalVars.h"
#include "GLUtils.h"


#define WINDOW_WIDTH	1000.0
#define WINDOW_HEIGHT	1000.0

//Referencia à classe filha.
class Ship;
class Projectile;

class Entity
{

private:
	GLFWwindow* window;

protected:
	int size;
	double mass; //F = m . a	-> F/m = a
	float hitPoints;
	
	std::vector<Force*> forces = std::vector<Force*>();
	std::vector<Force> instantaneousForces = std::vector<Force>();
	std::vector<std::pair<double, Force>> tempForces = std::vector<std::pair<double, Force>>();
	
	vec3 forcesSum;
	vec3 acceleration;
	vec3 linearSpeed;
	vec3 position;
	
	//Resistencia do elemento à sofrer rotações.
	float inertiaMoment;
	vec3 torque;
	vec3 angularAcceleration;
	vec3 angularSpeed;
	vec3 rotation;

	vec3 direction;

	Faction faction;
public:
	Entity();
	Entity(GLFWwindow* window, int size);

	void Init();
	virtual void Update(double t);
	virtual void Render() = 0;
	virtual void Destroy() = 0;;
	//void resize(int, int);

	void UpdateForces(double t);
	void UpdateAcceleration(double t);
	void UpdateSpeed(double t);
	void UpdatePosition(double t);

	void RemoveForce(Force* f);
	void AddForce(Force* f) { forces.push_back(f); }
	void AddTempForce(Force f, double timeToLive);
	void AddInstantaneousForce(Force f);
	void ClearInstantaneousForce();


	virtual void ReceiveDamage(float damage, DamageType damageType) { return; }

	//Funções para lidar com a colisão entre tipos colliders diferentes.
	virtual void CollideWith(Entity& otherEntity, glm::vec3 collisionPoint) { return; }
	virtual void CollideWithShip(Ship& otherShip, glm::vec3 collisionPoint) { return; }
	virtual void CollideWithProjectile(Projectile& otherProjectile, glm::vec3 collisionPoint) { return; }

	//Função chamada após ser constatada uma colisão, as coordenadas do collisionPoint
	//São coordenadas já projetadas.
	virtual void CollideAtProjection(Entity& otherEntity, glm::vec3 collisionPoint) = 0;

public:

	void SetPosition(vec3 position) { this->position = position; }
	void SetRotation(vec3 rotation) { this->rotation = rotation; }
	void SetInertia(float i) { inertiaMoment = i; }
	void SetMass(double m) { mass = m; }
	void SetLinearSpeed(vec3 newLinearSpeed) { this->linearSpeed = newLinearSpeed; }
	void SetAngularSpeed(vec3 newAngularSpeed) { this->angularSpeed = newAngularSpeed; }
	void SetFaction(Faction fact) { this->faction = fact;  }
	void SetHitPoints(float hp) { this->hitPoints = hp; }

	float GetHitPoints() { return this->hitPoints; }
	vec3 GetPosition() { return position; }
	vec3 GetRotation() { return rotation; }
	vec3* GetPositionRef() { return &position; }
	vec3* GetRotationRef() { return &rotation; }
	virtual mat4 GetMVP() { return mat4(); }
	virtual mat4 GetRotationMatrix() { return mat4(); }
	virtual mat4 GetScaleMatrix() { return mat4(); }

	double GetSpeed() { return linearSpeed.length(); }	
	double GetInertia() { return this->inertiaMoment; }
	double GetMass() { return this->mass; }
	vec3& GetLinearSpeed() { return this->linearSpeed; }
	vec3& GetAngularSpeed() { return this->angularSpeed; }
	vec3& GetLinearAcceleration() { return this->acceleration; }


	Faction GetFaction() { return this->faction; }
};