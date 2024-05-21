
#include "ShaderManager.h"
using namespace std;

ShaderManager* ShaderManager::shaderManagerInstance = nullptr;


ShaderManager::ShaderManager()
{

}

void ShaderManager::AddShader(SkillID skillId, Shader* shader)
{
    SkillShaderDict[skillId] = shader;
}

void ShaderManager::AddShader(ProjectileTypeID projectileId, Shader* shader)
{
    ProjectileShaderDict[projectileId] = shader;
}

void ShaderManager::AddShader(ShaderType typeShader, Shader* shader)
{
    ShaderDict[typeShader] = shader;
}

void ShaderManager::AddShader(int id, ShaderType typeShader, Shader* shader)
{
    std::map<int, Shader*>* Dict = GetSelector(typeShader);
    (*Dict)[id] = shader;
}

Shader* ShaderManager::GetShader(SkillID skillId)
{
    auto it = SkillShaderDict.find(skillId);
    if (it != SkillShaderDict.end()) {
        return it->second;
    }
    else {
        std::cout << "\nShader de SKILL requisitado não encontrado no dicionário.\n" << std::endl;
        return nullptr;
    }
}

Shader* ShaderManager::GetShader(ProjectileTypeID projectileId)
{
    auto it = ProjectileShaderDict.find(projectileId);
    if (it != ProjectileShaderDict.end()) {
        return it->second;
    }
    else {
        std::cout << "\nShader de SKILL requisitado não encontrado no dicionário.\n" << std::endl;
        return nullptr;
    }
}

Shader* ShaderManager::GetShader(ShaderType typeShader)
{
    auto it = ShaderDict.find(typeShader);
    if (it != ShaderDict.end()) {
        return it->second;
    }
    else {
        std::cout << "\nShader requisitado não encontrado no dicionário.\n" << std::endl;
        return nullptr;
    }
}

Shader* ShaderManager::GetShader(int id, ShaderType typeShader)
{
    std::map<int, Shader*>* Dict = GetSelector(typeShader);
    
    auto it = Dict->find(id);
    if (it != Dict->end()) {
        return it->second;
    }
    else {
        std::cout << "\nShader requisitado não encontrado no dicionário.\n" << std::endl;
        return nullptr;
    }
}

std::map<int, Shader*>* ShaderManager::GetSelector(ShaderType dict_type_id)
{
    switch (dict_type_id)
    {
    case ShaderType::SHADER_TYPE_TEXT:
        return &this->TextShaderDict;
    case ShaderType::SHADER_TYPE_SHIP:
        return &this->ShipShaderDict;
    case ShaderType::SHADER_TYPE_COLLIDER:
        return &this->ColliderShaderDict;
    case ShaderType::SHADER_TYPE_AXIS:
        return &this->AxisShaderDict;
    case ShaderType::SHADER_TYPE_INTERFACE:
        return &this->InterfaceShaderDict;
    default:
        break;
    }
}

