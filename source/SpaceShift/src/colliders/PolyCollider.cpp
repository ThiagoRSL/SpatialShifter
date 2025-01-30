
#include "PolyCollider.h"
#include "PointCollider.h"
using namespace std;

PolyCollider::PolyCollider()
{
    collidingRadius = GlobalVars::STANDARD_COLLIDING_RADIUS;
    DebugColliderColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	Vertexes = std::vector<glm::vec3>();
    Edges = std::vector<std::pair<unsigned int, unsigned int>>();
    TransformedVertexes = std::vector<glm::vec3>();
	TransformedEdges = std::vector<std::pair<glm::vec3, glm::vec3>>();
}

void PolyCollider::Init(Entity* entityRepresented, const char* modelPath)
{
    this->Controller = entityRepresented;
    bool res = ObjectLoader::LoadCollider(modelPath, Vertexes, Edges);

    // ERRADO
    if (GlobalVars::DEBUG_MODE)
    {
        //ColliderDebugShader = new Shader();
        //ColliderDebugShader->Compile("shader/debug/debugcollider.vert", "shader/debug/debugcollider.frag");
        //GenerateColliderDebugBuffers();
    }
}

bool PolyCollider::isColliding(const AbstractCollider& other) const
{
	return other.isCollidingWithPoly(*this);
}

// Implemente a lógica de detecção de colisão para CircCollider vs PolyCollider aqui
bool PolyCollider::isCollidingWithPoly(const PolyCollider& poly) const
{
	const std::vector<std::pair<glm::vec3, glm::vec3>>& edges = poly.GetEdges();
    
    //Verifica se há intersecção entre arestas.
    for (int i = 0; i < edges.size(); i++)
    {
        if (EdgeHasCollision(edges.at(i)))
            return true;
    }

    return false;    
}

// Implemente a lógica de detecção de colisão para CircCollider vs PolyCollider aqui
bool PolyCollider::isCollidingWithPoint(const PointCollider& point) const
{
    const std::vector<glm::vec3>& vertexes = point.GetTransformedVertexes();

    for (int i = 0; i < vertexes.size(); i++)
    {
        if (IsPointInside(vertexes.at(i)))
            return true;
    }

    return false;
}


bool PolyCollider::EdgeHasCollision(const std::pair<glm::vec3, glm::vec3> CollidingEdge) const
{
    vec3 p1 = CollidingEdge.first;
    vec3 p2 = CollidingEdge.second;

    for (int i = 0; i < TransformedEdges.size(); i++)
    {
        //Verifica se existe intersecção entre arestas.
        if (MathUtils::HasInterception(vec2(p1.x, p1.y), 
            vec2(p2.x, p2.y),
            vec2(TransformedEdges.at(i).first.x, TransformedEdges.at(i).first.y),
            vec2(TransformedEdges.at(i).second.x, TransformedEdges.at(i).second.y)))
        {
            return true;
        }
    }

    return false;
}

glm::vec3 PolyCollider::CollidingAt(const AbstractCollider& other) const
{
    return other.CollidingWithPolyAt(*this);
}

// Implemente a lógica de detecção de colisão para CircCollider vs PolyCollider aqui
glm::vec3 PolyCollider::CollidingWithPolyAt(const PolyCollider& poly) const
{
    const std::vector<std::pair<glm::vec3, glm::vec3>>& edges = poly.GetEdges();

    for (int i = 0; i < edges.size(); i++)
    {
        int collidedEdgeIndex = EdgeFindCollision(edges.at(i));
        if (collidedEdgeIndex >= 0)
        {
            //Este não é exatamente o ponto de colisão, na verdade é o ponto de intersecção.
            //Eles SÂO diferentes, pois quando as duas arestas colidem a intersecção entre elas
            //Só existe por que ambos os poligonos já compartilham o mesmo espaço.

            glm::vec2 collisionPoint = 
                MathUtils::FindIntersection(
                    vec2(edges.at(i).first),
                    vec2(edges.at(i).second),
                    vec2(TransformedEdges.at(collidedEdgeIndex).first),
                    vec2(TransformedEdges.at(collidedEdgeIndex).second)
                );
            if(collisionPoint == vec2(FLT_MAX, FLT_MAX))
                return vec3(FLT_MAX, FLT_MAX, FLT_MAX);

            return vec3(collisionPoint.x, collisionPoint.y, 0.0f);
        }
    }
    
    return vec3(FLT_MAX, FLT_MAX, FLT_MAX);
}

// Implemente a lógica de detecção de colisão para CircCollider vs PolyCollider aqui
glm::vec3 PolyCollider::CollidingWithPointAt(const PointCollider& point) const
{
    return point.CollidingWithPolyAt(*this); // Retorna o centro do "ponto".
}
//TODO: VERIFICAR SÓ se o menor está dentro do maior
//Verifica se há intersecção entre vertice e poligono.


bool PolyCollider::IsPointInside(const glm::vec3 Vertex) const
{

    //Para cada ponto, verifica qual ter intersecção impar com os edges.
    int pointIntersections = 0;
    for (int j = 0; j < TransformedEdges.size(); j++)
    {
        //Verifica se existe intersecção entre arestas.
        if (MathUtils::HasInterception(vec2(Vertex.x, Vertex.y),
            vec2(99999.0f, 99999.0f),//Ponto arbitrario que com toda certeza está fora do objeto
            vec2(TransformedEdges.at(j).first.x, TransformedEdges.at(j).first.y),
            vec2(TransformedEdges.at(j).second.x, TransformedEdges.at(j).second.y)))
        {
            pointIntersections++;
        }
    }
    if (pointIntersections % 2 == 1) return true;
    return false;
}


// Implemente a lógica de detecção de colisão para CircCollider vs PolyCollider aqui
glm::vec3 PolyCollider::CalculateCollisionDepth(const PolyCollider& poly)
{
    //Multiplas partes podem destar colidindo
    const std::vector<std::pair<glm::vec3, glm::vec3>>& edges = poly.GetEdges();
    std::vector<int> collidingEdges = std::vector<int>();

    for (int i = 0; i < edges.size(); i++)
    {
        int collidedEdgeIndex = EdgeFindCollision(edges.at(i));
        if (collidedEdgeIndex >= 0) collidingEdges.push_back(collidedEdgeIndex);
        if (collidedEdgeIndex >= 0)
        {
            //Este não é exatamente o ponto de colisão, na verdade é o ponto de intersecção.
            //Eles SÂO diferentes, pois quando as duas arestas colidem a intersecção entre elas
            //Só existe por que ambos os poligonos já compartilham o mesmo espaço.

            glm::vec2 collisionPoint =
                MathUtils::FindIntersection(
                    vec2(edges.at(i).first),
                    vec2(edges.at(i).second),
                    vec2(TransformedEdges.at(collidedEdgeIndex).first),
                    vec2(TransformedEdges.at(collidedEdgeIndex).second)
                );
            if (collisionPoint == vec2(FLT_MAX, FLT_MAX))
                return vec3(FLT_MAX, FLT_MAX, FLT_MAX);

            return vec3(collisionPoint.x, collisionPoint.y, 0.0f);
        }
    }

    return vec3(FLT_MAX, FLT_MAX, FLT_MAX);
}

int PolyCollider::EdgeFindCollision(const std::pair<glm::vec3, glm::vec3> CollidingEdge) const
{
    vec3 p1 = CollidingEdge.first;
    vec3 p2 = CollidingEdge.second;

    for (int i = 0; i < Edges.size(); i++)
    {
        if (MathUtils::HasInterception(vec2(p1.x, p1.y),
            vec2(p2.x, p2.y),
            vec2(TransformedEdges.at(i).first.x, TransformedEdges.at(i).first.y),
            vec2(TransformedEdges.at(i).second.x, TransformedEdges.at(i).second.y)))
        {
            return i;
        }
    }

    return -1;
}


void PolyCollider::Update(double deltaTime)
{
    SetDebugColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
    position = Controller->GetPosition() - CameraManager::GetInstance()->WorldPivot();
    rotation = Controller->GetRotation();

    TransformedVertexes.clear();
    TransformedEdges.clear();

    for (int i = 0; i < Vertexes.size(); i++)
    {
        TransformedVertexes.push_back(vec3(
            this->Controller->GetRotationMatrix() * (this->Controller->GetScaleMatrix() 
                * vec4(Vertexes[i].x, Vertexes[i].y, Vertexes[i].z, 1.0)) + vec4(position, 1.0)));
    }

    for (int i = 0; i < Edges.size(); i++)
    {
        TransformedEdges.push_back(std::pair<glm::vec3, glm::vec3>
            (TransformedVertexes.at(Edges.at(i).first - 1), TransformedVertexes.at(Edges.at(i).second - 1)));
    }
    
}

void PolyCollider::Render()
{
    ColliderDebugShader->setUniform(string("MVP"), CameraManager::GetInstance()->MVP()); //ModelViewProjection
    ColliderDebugShader->setUniform(string("Position"), position);
    ColliderDebugShader->setUniform(string("RotationMatrix"), this->Controller->GetRotationMatrix());
    ColliderDebugShader->setUniform(string("ScaleMatrix"), this->Controller->GetScaleMatrix());
    ColliderDebugShader->setUniform(string("DebugColor"), DebugColliderColor);

    // set var MVP on the shader
    ColliderDebugShader->RenderColliderDebug();

}
// bool isCollidingWithCirc(const CircCollider& circ);