
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
        std::cout << "\nShader de Projectile requisitado não encontrado no dicionário.\n" << std::endl;
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

Shader* ShaderManager::GetShipShader(ShipTypeId id)
{
    std::map<int, Shader*>* Dict = GetSelector(ShaderType::SHADER_TYPE_SHIP);
    Shader* ShipModelShader;

    auto it = Dict->find(static_cast<int>(id));
    if (it != Dict->end())
        ShipModelShader = it->second;
    else {
        std::cout << "\nShader requisitado não encontrado no dicionário (Com ID).\n" << std::endl;
        ShipModelShader = new Shader();

        printf("\n COMPILANDO SHADERS (1).");
        ShipModelShader->Compile("shader/ship/ship.vert", "shader/ship/ship.geom", "shader/ship/ship.frag");

        AddShader(static_cast<int>(id), ShaderType::SHADER_TYPE_SHIP, ShipModelShader);

        bool res = ObjectLoader::LoadObject(ShipBuilder::GetInstance()->GetShipModelPath(id), ShipModelShader->Vertexes,
            ShipModelShader->Uvs, ShipModelShader->Normals, ShipModelShader->Indexes);

        if (res)
            printf("Arquivo de modelo 3d carregado com sucesso!");
        else
            printf("Arquivo de modelo 3d não foi carregado com sucesso.");

        ShipModelShader->GenerateBuffers();
    }

    return ShipModelShader;
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


Shader* ShaderManager::GetParticleShader()
{
    if (this->particleShader == nullptr)
    {
        this->particleShader = new Shader();
        printf("\n COMPILANDO SHADERS (Particulas).");
        this->particleShader->Compile("shader/particle/particles.vert", "shader/particle/particles.geom", "shader/particle/particles.frag");
    }

    return this->particleShader; 
}

Shader* ShaderManager::GetParticleUpdateShader()
{
    if (this->particleUpdateShader == nullptr)
    {
        this->particleUpdateShader = new Shader();
        printf("\n COMPILANDO SHADERS (1).");
        this->particleUpdateShader->Compile("shader/particle/particles.comp");
    }

    return this->particleUpdateShader;
}