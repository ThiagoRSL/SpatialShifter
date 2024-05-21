
#include "PointCollider.h"
#include "PolyCollider.h"

using namespace std;

PointCollider::PointCollider()
{
    collidingRadius = GlobalVars::STANDARD_COLLIDING_RADIUS;
	Vertexes = std::vector<glm::vec3>();
    Edges = std::vector<std::pair<unsigned int, unsigned int>>();
    TransformedVertexes = std::vector<glm::vec3>();
}

void PointCollider::Init(Entity* entityRepresented, const char* colliderModelPath)
{
    this->Controller = entityRepresented;
    bool res = ObjectLoader::LoadCollider(colliderModelPath, Vertexes, Edges);

    /*if (GlobalVars::DEBUG_MODE)
    {
        ColliderDebugShader = new Shader();
        ColliderDebugShader->Compile("shader/debug/debugcollider.vert", "shader/debug/debugcollider.frag");
        GenerateColliderDebugBuffers();
    }*/
}

bool PointCollider::isColliding(const AbstractCollider& other) const
{
	return other.isCollidingWithPoint(*this);
}

// Implemente a lógica de detecção de colisão para CircCollider vs PolyCollider aqui
bool PointCollider::isCollidingWithPoly(const PolyCollider& poly) const
{
    return poly.isCollidingWithPoint(*this);
}

glm::vec3 PointCollider::CollidingAt(const AbstractCollider& other) const
{
    return other.CollidingWithPointAt(*this);
}

// Implemente a lógica de detecção de colisão para CircCollider vs PolyCollider aqui
glm::vec3 PointCollider::CollidingWithPolyAt(const PolyCollider& poly) const
{
    return Position;
}

void PointCollider::Update(double deltaTime)
{
    Position = Controller->GetPosition() - CameraManager::GetInstance()->WorldPivot();
    rotation = Controller->GetRotation();

    TransformedVertexes.clear();
    for (int i = 0; i < Vertexes.size(); i++)
    {
        TransformedVertexes.push_back(vec3(
            //this->Controller->GetRotationMatrix() * (this->Controller->GetScaleMatrix() * 
            vec4(Vertexes[i].x, Vertexes[i].y, Vertexes[i].z, 1.0) + vec4(Position, 1.0)));//);
    }
}