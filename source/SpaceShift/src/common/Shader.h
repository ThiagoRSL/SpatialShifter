#pragma once

#include "ObjectLoader.h"
#include "GLUtils.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class Shader
{
public:
    // the program ID
    unsigned int ID;
  
    // constructor reads and builds the shader
    Shader();
    // use/activate the shader
    //void use();
    // utility uniform functions
    std::vector<std::pair < unsigned int, unsigned int >> Edges;

    void Use();
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