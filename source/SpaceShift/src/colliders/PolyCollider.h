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

class PolyCollider : public AbstractCollider
{

private:

	vec3 position;
	vec3 rotation;

	std::vector<glm::vec3> Vertexes;
	std::vector<std::pair<unsigned int, unsigned int>> Edges;
	std::vector<glm::vec3> TransformedVertexes;
	std::vector<std::pair<glm::vec3, glm::vec3>> TransformedEdges;
	//Shaders do Debug das Bordas de Colisão.
	Shader* ColliderDebugShader;

public:
	
	PolyCollider();
	void GenerateBuffers();
	void Update(double deltaTime);
	void Render();
	void Init(Entity* entityRepresented, const char* modelPath);


	// TODO: Aprimorar colisão de poligonos...
	// - fazer verificação a frente do tempo.
	// - FIX: a colisão não é calculada quando poligono entra em outro, precisa verificar este cenário.
	bool isColliding(const AbstractCollider& other) const;
	bool isCollidingWithPoly(const PolyCollider& poly) const;
	bool isCollidingWithPoint(const PointCollider& point) const;
	//bool isCollidingWithPoint(const PointCollider& point) const;

	bool IsPointInside(const glm::vec3 Vertex) const;
	bool EdgeHasCollision(const std::pair<glm::vec3, glm::vec3> CollidingEdge) const;
	glm::vec3 CalculateCollisionDepth(const PolyCollider& poly);
	// bool isCollidingWithCirc(const CircCollider& circ);

	glm::vec3 CollidingAt(const AbstractCollider& other) const;
	glm::vec3 CollidingWithPolyAt(const PolyCollider& poly) const;
	glm::vec3 CollidingWithPointAt(const PointCollider& point) const;

	const std::vector<glm::vec3>& GetVertexes() const { return Vertexes; }
	const std::vector<glm::vec3>& GetTransformedVertexes() const { return TransformedVertexes; }
	const std::vector<std::pair<glm::vec3, glm::vec3>>& GetEdges() const { return TransformedEdges; };

private:
	//Returns -1 if hasn't collided, returns the edge index if it has collided.
	glm::vec3 CollidedOnWorldAt(const PolyCollider& poly, glm::vec3) const;
	int EdgeFindCollision(const std::pair<glm::vec3, glm::vec3> CollidingEdge) const;

	void GenerateColliderDebugBuffers();
	void GenerateColliderVertexes();

};