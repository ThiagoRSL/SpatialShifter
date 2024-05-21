
 #include "ShipBuilder.h"
using namespace std;

ShipBuilder* ShipBuilder::shipBuilderInstance = nullptr;

ShipBuilder::ShipBuilder()
{
    ShipDict = std::map<ShipTypeId, const char*>();
    ColliderDict = std::map<ShipTypeId, const char*>();
    

    ShipDict[ShipTypeId::STANDARD_SHIP_ID] = "../source/SpaceShift/assets/ships//test.obj";
    ColliderDict[ShipTypeId::STANDARD_SHIP_ID] = "../source/SpaceShift/assets/ships//test_collider.obj";

    ShipDict[ShipTypeId::MICRO_SHIP_ID] = "../source/SpaceShift/assets/ships//micro_ship.obj";
    ColliderDict[ShipTypeId::MICRO_SHIP_ID] = "../source/SpaceShift/assets/ships//micro_ship_collider.obj";

    ShipDict[ShipTypeId::NANO_SHIP_ID] = "../source/SpaceShift/assets/ships//nano_ship.obj";
    ColliderDict[ShipTypeId::NANO_SHIP_ID] = "../source/SpaceShift/assets/ships//nano_ship_collider.obj";

    ShipDict[ShipTypeId::ZWEIHANDER_SHIP_ID] = "../source/SpaceShift/assets/ships//zweihander_alt.obj";
    ColliderDict[ShipTypeId::ZWEIHANDER_SHIP_ID] = "../source/SpaceShift/assets/ships//zweihander_alt_collider.obj";

    ProjectileDict[ProjectileTypeID::BALLISTIC_PROJECTILE_ID] = "../source/SpaceShift/assets/projectile//common_missile.obj";
    ProjectileColliderDict[ProjectileTypeID::BALLISTIC_PROJECTILE_ID] = "../source/SpaceShift/assets/projectile//common_missile_collider.obj";

}

const char* ShipBuilder::GetShipModelPath(ShipTypeId shipTypeId)
{
    // Verificando se um elemento existe no dicionário
    if (ShipDict.find(shipTypeId) != ShipDict.end()) {
        return ShipDict[shipTypeId];
    }
    else 
    {
        std::cout << "\nShader requisitado não encontrado no dicionário.\n" << std::endl;
        return nullptr;
    }
}
const char* ShipBuilder::GetColliderModelPath(ShipTypeId shipTypeId)
{
    // Verificando se um elemento existe no dicionário
    if (ColliderDict.find(shipTypeId) != ColliderDict.end()) {
        return ColliderDict[shipTypeId];
    }
    else
    {
        std::cout << "\nShader requisitado não encontrado no dicionário.\n" << std::endl;
        return nullptr;
    }
}
const char* ShipBuilder::GetProjectileModelPath(ProjectileTypeID projectileTypeId)
{
    // Verificando se um elemento existe no dicionário
    if (ProjectileDict.find(projectileTypeId) != ProjectileDict.end()) {
        return ProjectileDict[projectileTypeId];
    }
    else
    {
        std::cout << "\nShader requisitado não encontrado no dicionário.\n" << std::endl;
        return nullptr;
    }
}

const char* ShipBuilder::GetProjectileColliderPath(ProjectileTypeID projectileTypeId)
{
    // Verificando se um elemento existe no dicionário
    if (ProjectileColliderDict.find(projectileTypeId) != ProjectileColliderDict.end()) {
        return ProjectileColliderDict[projectileTypeId];
    }
    else
    {
        std::cout << "\nShader requisitado não encontrado no dicionário.\n" << std::endl;
        return nullptr;
    }
}

