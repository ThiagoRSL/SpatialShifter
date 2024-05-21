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


ProjectileModel::ProjectileModel(Projectile* ProjectileController)
{
	this->Controller = ProjectileController;

	position = ProjectileController->GetPosition() - CameraManager::GetInstance()->WorldPivot();
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
	modelViewProjectionMatrix = CameraManager::GetInstance()->MVP();

	ProjectileShader = ShaderManager::GetInstance()->GetShader(this->Controller->GetProjectileType());
	if (ProjectileShader == nullptr)
	{
		ProjectileShader = new Shader();
		printf("\n COMPILANDO SHADERS (1).");
		ProjectileShader->Compile("shader/projectile/ballistic.vert", "shader/projectile/ballistic.geom", "shader/projectile/ballistic.frag"); //"shader/ship/ship.geom",
		ShaderManager::GetInstance()->AddShader(this->Controller->GetProjectileType(), ProjectileShader);
		ProjectileShader->setUniform(string("MVP"), CameraManager::GetInstance()->MVP()); //ModelViewProjection
	}

	bool res = ObjectLoader::LoadObject(ShipBuilder::GetInstance()->GetProjectileModelPath(ProjectileType), Vertexes, Uvs, Normals, Indexes);

	if (res)
		printf("Arquivo de modelo 3d carregado com sucesso!");
	else
		printf("Arquivo de modelo 3d não foi carregado com sucesso.");

	glGenVertexArrays(1, &VaoID);
	glBindVertexArray(VaoID);
	VboID = new GLuint[5];
	glGenBuffers(5, VboID);
	glBindVertexArray(0);

	GenerateBuffers();
}

void ProjectileModel::Render()
{
	ProjectileShader->setUniform(string("Position"), position - CameraManager::GetInstance()->WorldPivot());
	ProjectileShader->setUniform(string("Color"), vec4(1.0f, 0.0f, 1.0f, 1.0f));
	ProjectileShader->setUniform(string("RotationMatrix"), rotationMatrix);
	ProjectileShader->setUniform(string("ScaleMatrix"), scaleMatrix);
	ProjectileShader->setUniform(string("DestructionAnimationParam"), (float)MathUtils::AnimationSmoothTime(this->Controller->GetDestroyedTime() / 1.5));
	ProjectileShader->setUniform(string("Destroyed"), this->Controller->IsDestroyed());

	// set var MVP on the shader
	ProjectileShader->Use();
	glBindVertexArray(VaoID);
	glDrawArrays(GL_TRIANGLES, 0, Indexes.size());
	glBindVertexArray(0);
	glUseProgram(0);
}


void ProjectileModel::Update(double deltaTime)
{
	position = Controller->GetPosition();
	rotationMatrix = Controller->GetRotationMatrix();
	scaleMatrix = Controller->GetScaleMatrix();

	CameraManager* cam = CameraManager::GetInstance();

	modelViewProjectionMatrix = cam->ProjectionMatrix() * cam->ViewMatrix();

	this->GenerateParticles();
}

void ProjectileModel::GenerateBuffers()
{

	// Definindo as cores dos vértices.
	for (int i = 0; i <= Indexes.size(); i++)
	{
		Colors.push_back(ProjectileModelColor);
	}	

	glBindVertexArray(VaoID);

	//Habilita transparência
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glBindBuffer(GL_ARRAY_BUFFER, VboID[0]);
	glBufferData(GL_ARRAY_BUFFER, Vertexes.size() * sizeof(vec3), (GLvoid*)&Vertexes[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
	glEnableVertexAttribArray(0);  // VertexPosition -> layout 0 in the VS
	 
	glBindBuffer(GL_ARRAY_BUFFER, VboID[1]);
	glBufferData(GL_ARRAY_BUFFER, Uvs.size() * sizeof(vec2), (GLvoid*)&Uvs[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
	glEnableVertexAttribArray(1);  // UVs -> layout 1 in the VS

	glBindBuffer(GL_ARRAY_BUFFER, VboID[2]);
	glBufferData(GL_ARRAY_BUFFER, Normals.size() * sizeof(vec3), (GLvoid*)&Normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)2, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
	glEnableVertexAttribArray(2);  // Normals -> layout 2 in the VS

	glBindBuffer(GL_ARRAY_BUFFER, VboID[3]);
	glBufferData(GL_ARRAY_BUFFER, Colors.size() * sizeof(vec4), (GLvoid*)&Colors[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)3, 4, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
	glEnableVertexAttribArray(3);  // Colors -> layout 3 in the VS

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboID[4]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indexes.size() * sizeof(int), (GLvoid*)&Indexes[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}


void ProjectileModel::GenerateParticles()
{

}
