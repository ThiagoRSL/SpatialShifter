#pragma once

#include <vector>
#include <iostream>

#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "GLUtils.h"
#include "../common/ObjectLoader.h"
#include "../Utils/MathUtils.h"
#include "../entities/Entity.h"
#include "AbstractCollider.h"
#include "../manager/CameraManager.h"

class PolyCollider;

class PointCollider : public AbstractCollider
{

private:
	vec3 Position;
	vec3 rotation;

	std::vector<std::pair<unsigned int, unsigned int>> Edges;
	std::vector<glm::vec3> Vertexes;
	std::vector<glm::vec3> TransformedVertexes;

public:

	PointCollider();
	void Init(Entity* entityRepresented, const char* colliderModelPath);
	void Update(double deltaTime);

	bool isColliding(const AbstractCollider& other) const;
	bool isCollidingWithPoly(const PolyCollider& poly) const;
	bool isCollidingWithPoint(const PointCollider& point) const { return false; } //TODO: Implementar lógica baseada em raio.

	glm::vec3 CollidingAt(const AbstractCollider& other) const;
	glm::vec3 CollidingWithPolyAt(const PolyCollider& poly) const;
	glm::vec3 CollidingWithPointAt(const PointCollider& point) const { return Position; }

	float GetCollidingRadius() { return this->collidingRadius; }
	void SetCollidingRadius(float r) { this->collidingRadius = r; }


	const std::vector<glm::vec3>& GetTransformedVertexes() const { return TransformedVertexes; }
};