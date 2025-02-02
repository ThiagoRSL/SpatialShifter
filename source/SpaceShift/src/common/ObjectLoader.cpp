
#include "ObjectLoader.h"
#include "GLUtils.h"

#include <windows.h>
#include <fstream>
#include <iostream>
#include <string>

using std::cout;
using std::endl;

bool ObjectLoader::LoadObject(const char* path, std::vector < glm::vec3 >& out_vertexes, std::vector < glm::vec2 >& out_uvs, std::vector < glm::vec3 >& out_normals, std::vector < unsigned int >& out_indexes)
{
    // Declara as variáveis para leitura.
    std::vector< unsigned int > vertexesIndexes, uvsIndexes, normalsIndexes;
    std::vector< glm::vec3 > vertexesTemp;
    std::vector< glm::vec2 > uvsTemp;
    std::vector< glm::vec3 > normalsTemp;

    FILE* file;
    fopen_s(&file, path, "rb");
    if (file == NULL)
    {
        printf("Não foi possível abrir o arquivo. \n Caminho: '%s'. (Object)\n", path);
        return false;
    }

    while (true)
    {
        char lineHeader[1024];
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break; // EOF = End Of File. Quit the loop.

        // else : parse lineHeader
        if (strcmp(lineHeader, "v") == 0)
        {
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            vertexesTemp.push_back(vertex);
        }
        else if (strcmp(lineHeader, "vt") == 0)
        {
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y);
            uvsTemp.push_back(uv);
        }
        else if (strcmp(lineHeader, "vn") == 0)
        {
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            normalsTemp.push_back(normal);
        }
        else if (strcmp(lineHeader, "f") == 0)
        {
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
            if (matches != 9)
            {
                printf("File can't be read by our simple parser : ( Try exporting with other options\n");
                return false;
            }
            vertexesIndexes.push_back(vertexIndex[0]);
            vertexesIndexes.push_back(vertexIndex[1]);
            vertexesIndexes.push_back(vertexIndex[2]);
            uvsIndexes.push_back(uvIndex[0]);
            uvsIndexes.push_back(uvIndex[1]);
            uvsIndexes.push_back(uvIndex[2]);
            normalsIndexes.push_back(normalIndex[0]);
            normalsIndexes.push_back(normalIndex[1]);
            normalsIndexes.push_back(normalIndex[2]);
        }
        else if (strcmp(lineHeader, "#") == 0)
        {
            //printf("Encontrei um comentário.\n");
        }
        else
        {
            //printf("OMG!!! found a DISGUTING: '%s'. \n", lineHeader);
        }
    }

    // For each vertex of each triangle
    for (unsigned int i = 0; i < vertexesIndexes.size(); i++)
    {
        unsigned int vertexIndex = vertexesIndexes[i];
        glm::vec3 vertex = vertexesTemp[vertexIndex - 1];
        out_vertexes.push_back(vertex);        
        out_indexes.push_back(vertexIndex);
    }

    // For each vertex of each triangle
    for (unsigned int i = 0; i < uvsIndexes.size(); i++)
    {
        unsigned int uvIndex = uvsIndexes[i];
        glm::vec2 uv = uvsTemp[uvIndex - 1];
        out_uvs.push_back(uv);
    }

    // For each vertex of each triangle
    for (unsigned int i = 0; i < normalsIndexes.size(); i++)
    {
        unsigned int normalIndex = normalsIndexes[i];
        glm::vec3 normal = normalsTemp[normalIndex - 1];
        out_normals.push_back(normal);
    }
    fclose(file);
    return true;
}

bool ObjectLoader::LoadCollider(const char* path, std::vector < glm::vec3 >& colliderVertexes, std::vector<std::pair < unsigned int, unsigned int >>& colliderEdges)
{
    // Declara as variáveis para leitura.
    std::vector< glm::vec3 > vertexesTemp;

    bool isGroup;
    FILE* file;
    fopen_s(&file, path, "rb");
    if (file == NULL)
    {
        printf("Não foi possível abrir o arquivo. \n Caminho: '%c'. (Collider)\n", path);
        return false;
    }
    while (true)
    {
        char lineHeader[1024];
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break; // EOF = End Of File. Quit the loop.

        // else : parse lineHeader
        if (strcmp(lineHeader, "v") == 0)
        {
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            colliderVertexes.push_back(vertex);
        }
        //Encontrou um lado entre os vertices.
        else if (strcmp(lineHeader, "l") == 0)
        {
            int v1, v2;
            fscanf(file, "%i %i\n", &v1, &v2);
            colliderEdges.push_back(std::pair<int, int>(v1, v2));
        }
        else if (strcmp(lineHeader, "#") == 0)
        {
            //printf("Encontrei um comentário.\n");
        }
        else
        {
            //printf("OMG!!! found a DISGUTING: '%s'. \n", lineHeader);
        }
    }
    fclose(file);
    //printf("\n\n\nVertices dos colisores: %i", colliderVertexes.size());
    return true;
}