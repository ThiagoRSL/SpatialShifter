#pragma once

#include "ObjectLoader.h"
#include "GLUtils.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class InputManager;

class Shader
{
public:
    // the program ID
    unsigned int ID;
    GLuint VaoID;
    GLuint* VboID;
  
    // constructor reads and builds the shader
    Shader();
    // use/activate the shader
    //void use();
    // utility uniform functions

    std::vector< vec3 > Vertexes;
    std::vector< vec2 > Uvs;
    std::vector< vec3 > Normals;
    std::vector< vec4 > Colors;
    std::vector<unsigned int> Indexes;

    std::vector<std::pair < unsigned int, unsigned int >> Edges;

    void Use();

    void GenerateBuffers();
    void GenerateSkillBuffers();
    void Render();

    void GenerateMouseBuffers();
    void RenderMouse();

    void GenerateColliderBuffers();
    void RenderColliderDebug();


    void Compile(const char* computePath);
    void Compile(const char* vertexPath, const char* fragmentPath);
    void Compile(const char* vertexPath, const char* geometryPath, const char* fragmentPath);

    void setUniform(const std::string& name, bool value);
    void setUniform(const std::string& name, int value);
    void setUniform(const std::string& name, float value);
    void setUniform(const std::string& name, double value);
    void setUniform(const std::string& name, float x, float y);
    void setUniform(const std::string& name, float x, float y, float z);
    void setUniform(const std::string& name, float x, float y, float z, float w);
    void setUniform(const std::string& name, const vec2& v);
    void setUniform(const std::string& name, const vec3& v);
    void setUniform(const std::string& name, const vec4& v);
    void setUniform(const std::string& name, const mat4& m);
    void setUniform(const std::string& name, const mat3& m);
    void setUniform(const std::string& name, GLuint value);

};