#include "ShipModel.h"

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
#include "../entities/Ship.h"

using namespace std;

ColliderModel::ColliderModel(Entity* controller)
{
	this->Controller = controller;

	position = controller->GetPosition() - CameraManager::GetInstance()->WorldPivot();
	rotation = controller->GetRotation();
	scale = vec3(1.0f);
} 

void ColliderModel::SetController(Entity* controller)
{
	this->Controller = controller;
	rotation = controller->GetRotation();
}

/*void ColliderModel::LoadCollider(const char* colliderPath)
{
	ObjectLoader::LoadCollider(colliderPath, Vertexes, Edges);
}*/

void ColliderModel::Init()
{
	/// matrices setup
	modelMatrix = mat4(); // identity

	ColliderShader = ShaderManager::GetInstance()->GetShader(ShaderType::SHADER_TYPE_COLLIDER);
	if (ColliderShader == nullptr)
	{
		ColliderShader = new Shader();
		printf("\n COMPILANDO SHADERS (1).");
		ColliderShader->Compile("shader/ship/ship.vert", "shader/ship/ship.geom", "shader/ship/ship.frag");
		ShaderManager::GetInstance()->AddShader(ShaderType::SHADER_TYPE_COLLIDER, ColliderShader);
		ColliderShader->GenerateColliderBuffers();
	}

}

void ColliderModel::Render()
{
	ColliderShader->setUniform(string("MVP"), CameraManager::GetInstance()->RefMVP()); //ModelViewProjection
	ColliderShader->setUniform(string("Position"), position);
	ColliderShader->setUniform(string("RotationMatrix"), rotationMatrix);
	ColliderShader->setUniform(string("ScaleMatrix"), scaleMatrix);
	ColliderShader->setUniform(string("Color"), vec4(1.0f));
	// set var MVP on the shader
	ColliderShader->Render();
}

void ColliderModel::Update(double deltaTime)
{
	position = Controller->GetPosition() - CameraManager::GetInstance()->WorldPivot();
	rotation = Controller->GetRotation();
	

	// Crie a matriz de escala
	scaleMatrix = glm::scale(glm::mat4(1.0f), scale);

	/*glm::vec3 angles = glm::radians(glm::vec3(rotation.x, rotation.y, rotation.z));

		// Cria um quaternion para cada rotação.
	glm::quat q1 = glm::angleAxis(angles.x, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::quat q2 = glm::angleAxis(angles.y, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::quat q3 = glm::angleAxis(angles.z, glm::vec3(0.0f, 0.0f, 1.0f));

	// Combina os quaternions, a rotação no eixo Z prece as demais pois a operação de rotações em R3 não é COMUTATIVA.
	glm::quat q = q3 * q2 * q1;

	rotationMatrix = glm::mat4(q); */

	rotationMatrix = mat4(); // identity

	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.z), vec3(0.0f, 0.0f, 1.0f));
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.y), vec3(0.0f, 1.0f, 0.0f));
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.x), vec3(1.0f, 0.0f, 0.0f));


	//this->GenerateParticles();
}

