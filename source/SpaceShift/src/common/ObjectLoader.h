#pragma once

#include <GL/glew.h>

#include <GLFW\glfw3.h>
#include "Scene.h"
#include <vector>
#include "../utils/MathUtils.h"

class ObjectLoader
{
    public:
        static bool LoadObject(const char* path, std::vector < glm::vec3 >& out_vertexes, std::vector < glm::vec2 >& out_uvs, std::vector < glm::vec3 >& out_normals, std::vector < unsigned int >& out_indexes);
        static bool LoadCollider(const char* path, std::vector < glm::vec3 >& colliderVertexes, std::vector<std::pair < unsigned int, unsigned int >>& colliderEdges);

};