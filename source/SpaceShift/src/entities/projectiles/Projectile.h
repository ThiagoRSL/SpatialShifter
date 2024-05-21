#pragma once

#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <vector>

#include <iostream>
#include "TextureManager.h"
#include "../Entity.h"
#include "../../manager/RenderManager.h"
#include "../../manager/UpdateManager.h"
#include "../../manager/CollisionManager.h"
#include "../../colliders/PointCollider.h"
#include "../../manager/ShaderManager.h"
#include "../../builders/ShipBuilder.h"
#include "../../models/ProjectileModel.h"
#include "GLUtils.h"
#include "../../utils/GlobalVars.h"


//Referencia à classe irmã.
class Ship;

class Projectile : public Entity
{

protected:
	ProjectileTypeID ProjectileType;
	ProjectileModel Model;
	PolyCollider* Collider;
	Entity* source;

	mat4 scaleMatrix;
	mat4 rotationMatrix;

	bool destroyed;
	double destroyedTime;
	float damage;
	DamageType damageType;

public:
	Projectile(Entity* source, float scale, float mass, float damage, DamageType damageType, float hitPoints,
		vec3 position, vec3 linearSpeed, Force f, ProjectileTypeID projectileTypeId, vec3 rotation, Faction fact);
	//Projectile(vec3 position, vec3 diretion, vec3 linearSpeed, Force boost);

	void Update(double t);
	void Init();
	void Render();
	void Destroy();
	void Erase();

	//Funções para lidar com a colisão entre tipos colliders diferentes.
	virtual void CollideWith(Entity& otherEntity, glm::vec3 collisionPoint);
	virtual void CollideWithShip(Ship& otherShip, glm::vec3 collisionPoint);
	virtual void CollideWithProjectile(Projectile& otherProjectile, glm::vec3 collisionPoint);

	virtual void CollideAtProjection(Entity& otherEntity, glm::vec3 collisionPoint);

	void SetDamage(float damage, DamageType damageType) { this->damage = damage; this->damageType = damageType; };
	Entity* GetSource() { return this->source; }

	bool IsDestroyed() { return this->destroyed; }
	double GetDestroyedTime() { return this->destroyedTime; }
	ProjectileTypeID GetProjectileType() { return this->ProjectileType; }
	mat4 GetScaleMatrix() { return this->scaleMatrix; }; 
	mat4 GetRotationMatrix() { return this->rotationMatrix; }
private:
	void InitCollider();

};