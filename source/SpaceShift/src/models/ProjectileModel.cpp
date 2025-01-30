#include "ProjectileModel.h"

#include <windows.h>
#include <iostream>
#include <string>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "GLUtils.h"
#include "TextureManager.h"
#include "../common/ObjectLoader.h"
#include "../manager/CameraManager.h"
#include "../manager/ShaderManager.h"
#include "../builders/ShipBuilder.h"
#include "../entities/projectiles/Projectile.h"

using namespace std;


ProjectileModel::~ProjectileModel()
{
	this->window = nullptr;
	this->Controller = nullptr;
	this->ProjectileShader; // TODO: Resolver
	this->AxisShader; // TODO: Resolver
}

ProjectileModel::ProjectileModel(Projectile* ProjectileController)
{
	this->Controller = ProjectileController;

	position = ProjectileController->GetPosition();
	rotationMatrix = ProjectileController->GetRotationMatrix();
	scaleMatrix = ProjectileController->GetScaleMatrix();
} 

void ProjectileModel::SetProjectileController(Projectile* Projectile)
{
	this->Controller = Projectile;
	rotation = Projectile->GetRotation();
}

void ProjectileModel::Init()
{
	ProjectileTypeID ProjectileType = this->Controller->GetProjectileType();
	int projectileTypeIdInt = static_cast<int>(ProjectileType);
	/// matrices setup
	modelMatrix = mat4(); // identity

	ProjectileShader = ShaderManager::GetInstance()->GetShader(this->Controller->GetProjectileType());
	if (ProjectileShader == nullptr)
	{
		ProjectileShader = new Shader();
		printf("\n COMPILANDO SHADERS (1).");
		ProjectileShader->Compile("shader/projectile/ballistic.vert", "shader/projectile/ballistic.geom", "shader/projectile/ballistic.frag"); //"shader/ship/ship.geom",
		
		ShaderManager::GetInstance()->AddShader(this->Controller->GetProjectileType(), ProjectileShader);

		bool res = ObjectLoader::LoadObject(ShipBuilder::GetInstance()->GetProjectileModelPath(ProjectileType), ProjectileShader->Vertexes, 
			ProjectileShader->Uvs, ProjectileShader->Normals, ProjectileShader->Indexes);

		if (res)
			printf("Arquivo de modelo 3d carregado com sucesso!");
		else
			printf("Arquivo de modelo 3d não foi carregado com sucesso.");

		ProjectileShader->GenerateBuffers();
	}
}

void ProjectileModel::Render()
{
	ProjectileShader->setUniform(string("MVP"), CameraManager::GetInstance()->MVP()); //ModelViewProjection
	ProjectileShader->setUniform(string("Position"), position - CameraManager::GetInstance()->WorldPivot());
	ProjectileShader->setUniform(string("Color"), vec4(1.0f, 0.0f, 1.0f, 1.0f));
	ProjectileShader->setUniform(string("RotationMatrix"), rotationMatrix);
	ProjectileShader->setUniform(string("ScaleMatrix"), scaleMatrix);
	ProjectileShader->setUniform(string("ZoomFactor"), CameraManager::GetInstance()->GetZoomFactor());
	ProjectileShader->setUniform(string("DestructionAnimationParam"), (float)MathUtils::AnimationSmoothTime(this->Controller->GetDestroyedTime() / 1.5));
	ProjectileShader->setUniform(string("Destroyed"), this->Controller->IsDestroyed());

	// set var MVP on the shader
	ProjectileShader->Render();
}


void ProjectileModel::Update(double deltaTime)
{
	position = Controller->GetPosition();
	rotationMatrix = Controller->GetRotationMatrix();
	scaleMatrix = Controller->GetScaleMatrix();

	CameraManager* cam = CameraManager::GetInstance();

	this->GenerateParticles();
}

void ProjectileModel::GenerateParticles()
{

}
