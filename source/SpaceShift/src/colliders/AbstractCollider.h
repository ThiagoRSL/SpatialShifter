#pragma once

#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <vector>
#include <iostream>

#include "TextureManager.h"
#include "../models/ColliderModel.h"
#include "../common/ObjectLoader.h"
#include "../Utils/MathUtils.h"
#include "../entities/Entity.h"
#include "GLUtils.h"

class PolyCollider;
class PointCollider;

class AbstractCollider : public RenderModel
{

private:
protected:
	ColliderModel ColliderModel;
	Entity* Controller;

	float collidingRadius;
	//std::vector<Force*> point = std::vector<Force*>();
	vec4 DebugColliderColor;

public:
	~AbstractCollider() { this->Controller = nullptr; }
	AbstractCollider() { ; }

	virtual bool isColliding(const AbstractCollider& other) const = 0;
	virtual bool isCollidingWithPoly(const PolyCollider& poly) const = 0;
	virtual bool isCollidingWithPoint(const PointCollider& point) const = 0;

	virtual glm::vec3 CollidingAt(const AbstractCollider& other) const = 0;
	virtual glm::vec3 CollidingWithPolyAt(const PolyCollider& poly) const = 0;
	virtual glm::vec3 CollidingWithPointAt(const PointCollider& point) const = 0;

	//	virtual bool isColliding(const AbstractCollider& other) const { return false; }
	//virtual bool isCollidingWithPoly(const PolyCollider& poly) const { return false; }
	//virtual bool isCollidingWithCirc(const CircCollider& circ) const = 0;

	// mesh virtual functions
	//void init();
	//virtual void Update(double t);
	//void resize(int, int);
	Entity* GetEntity() { return this->Controller; }
	void SetEntity(Entity* e) { this->Controller = e; }

	vec4 GetDebugColor() { return this->DebugColliderColor; }
	void SetDebugColor(vec4 color) { this->DebugColliderColor = color; }

	float GetCollidingRadius() { return this->collidingRadius; }
	void SetCollidingRadius(float r) { this->collidingRadius = r; }

};