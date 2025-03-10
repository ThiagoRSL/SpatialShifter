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

ShipModel::ShipModel(Ship* ShipController)
{
	this->Controller = ShipController; 
	this->ParticlesVboID = nullptr;

	position = ShipController->GetPosition() - CameraManager::GetInstance()->WorldPivot();
	rotation = ShipController->GetRotation();
	scale = vec3(1.0f);
	MaxParticlesNumber = 25;
	MaxTimeToLive = 0.3f;
	NextParticleIndex = 0;
	ParticleNumber = 0;

	ParticlePosition = new vec4[MaxParticlesNumber];
	ParticleVelocity = new vec4[MaxParticlesNumber];
	ParticleColor = new vec4[MaxParticlesNumber];
	ParticleTTL = new float[MaxParticlesNumber];
	ParticleIndex = new int[MaxParticlesNumber];
} 

void ShipModel::SetShipController(Ship* ship)
{
	this->Controller = ship;
	rotation = ship->GetRotation();
}

void ShipModel::Init()
{
	/// matrices setup
	modelMatrix = mat4(); 

	ShipModelShader = ShaderManager::GetInstance()->GetShipShader(this->Controller->GetShipType());
	ParticleShader = ShaderManager::GetInstance()->GetParticleShader();
	ParticleUpdateShader = ShaderManager::GetInstance()->GetParticleUpdateShader();
	ParticleTextureIndex = TextureManager::Inst()->GetTexture(GlobalPaths::PARTICLE_ENGINE_PATH);

	GenerateBuffers();
}

void ShipModel::Render()
{
	// TODO: Find proper values for mantain particle size based on camera size, 
	// Should do math properly for that.
	ParticleShader->Use();
	ParticleShader->setUniform(string("ParticleSize"), 0.01f - (0.007f * CameraManager::GetInstance()->GetSizeRelative()));
	ParticleShader->setUniform(string("ColorDegradation"), vec3(1.0f, 0.05f, 0.05f));
	ParticleShader->setUniform(string("MaxTTL"), MaxTimeToLive);
	ParticleShader->setUniform(string("RotationMatrix"), rotationMatrix);
	ParticleShader->setUniform(string("CamTrans"), CameraManager::GetInstance()->WorldPivot());
	ParticleShader->setUniform(string("MVP"), CameraManager::GetInstance()->MVP()); //ModelViewProjection
	ParticleShader->setUniform(string("TimeNow"), UpdateManager::GetInstance()->GetTimeCounter());

	ShipModelShader->Use();
	ShipModelShader->setUniform(string("MVP"), CameraManager::GetInstance()->RefMVP()); //ModelViewProjection
	ShipModelShader->setUniform(string("Position"), position);
	ShipModelShader->setUniform(string("RotationMatrix"), rotationMatrix);
	ShipModelShader->setUniform(string("ScaleMatrix"), scaleMatrix);
	ShipModelShader->setUniform(string("Color"), this->ShipModelColor);
	ShipModelShader->setUniform(string("DestructionAnimationParam"), this->Controller->GetDestroyedTime());
	ShipModelShader->setUniform(string("ShipState"), static_cast<int> (this->Controller->GetShipState()));
	// set var MVP on the shader;


	//Atualiza as posi��es das Particles
	/*ParticleUpdateShader->Use();
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ParticlesVboID[0]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ParticlesVboID[1]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ParticlesVboID[2]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ParticlesVboID[3]);
	glDispatchCompute(MaxParticlesNumber, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	glBindVertexArray(0);
	glUseProgram(0);*/

	this->GenerateParticles();	

	//Renderiza as Particulas
	TextureManager::Inst()->BindTexture(ParticleTextureIndex);

	ParticleShader->Use();
	ParticleShader->RenderParticles(ParticlesVaoID, MaxParticlesNumber);//*/

	//Renderiza a Nave
	glActiveTexture(GL_TEXTURE0);

	ShipModelShader->Use();
	ShipModelShader->Render();
}


void ShipModel::Update(double deltaTime)
{
	position = Controller->GetPosition() - CameraManager::GetInstance()->WorldPivot();
	rotation = Controller->GetRotation();

	//ParticleUpdateShader->setUniform(string("DeltaTime"), (float) deltaTime);
	scaleMatrix = glm::scale(glm::mat4(1.0f), scale);


	
	/*glm::vec3 angles = glm::radians(glm::vec3(rotation.x, rotation.y, rotation.z));

		// Cria um quaternion para cada rota��o.
	glm::quat q1 = glm::angleAxis(angles.x, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::quat q2 = glm::angleAxis(angles.y, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::quat q3 = glm::angleAxis(angles.z, glm::vec3(0.0f, 0.0f, 1.0f));

	// Combina os quaternions, a rota��o no eixo Z prece as demais pois a opera��o de rota��es em R3 n�o � COMUTATIVA.
	glm::quat q = q3 * q2 * q1;

	rotationMatrix = glm::mat4(q); */

	rotationMatrix = mat4(); // identity

	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.z), vec3(0.0f, 0.0f, 1.0f));
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.y), vec3(0.0f, 1.0f, 0.0f));
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.x), vec3(1.0f, 0.0f, 0.0f));

	CameraManager* cam = CameraManager::GetInstance();
}

void ShipModel::GenerateBuffers()
{
	glGenVertexArrays(1, &ParticlesVaoID);
	glBindVertexArray(ParticlesVaoID);
	ParticlesVboID = new GLuint[5];
	glGenBuffers(5, ParticlesVboID);

	glBindBuffer(GL_ARRAY_BUFFER, ParticlesVboID[0]);
	glBufferData(GL_ARRAY_BUFFER, MaxParticlesNumber * sizeof(vec4), nullptr, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, ParticlesVboID[1]);
	glBufferData(GL_ARRAY_BUFFER, MaxParticlesNumber * sizeof(vec4), nullptr, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, ParticlesVboID[2]);
	glBufferData(GL_ARRAY_BUFFER, MaxParticlesNumber * sizeof(vec4), nullptr, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, ParticlesVboID[3]);
	glBufferData(GL_ARRAY_BUFFER, MaxParticlesNumber * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ParticlesVboID[4]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, MaxParticlesNumber * sizeof(int), nullptr, GL_DYNAMIC_DRAW);

	glBindVertexArray(0);
}

void ShipModel::GenerateParticles()
{
	float thrustProportion = this->Controller->GetThrustIntensity() / this->Controller->GetThrustIntensityMax();
	if (thrustProportion == 0.0f || (int)(thrustProportion * 25.0f) == 0) return;

	int oldSize = NextParticleIndex;
	int createdParticles = 0;

	mat4 rotationMatrix = Controller->GetRotationMatrix();
	vec4* ParticleColor2 = new vec4[MaxParticlesNumber];

	int numberNewParticles = (10 + (int)(thrustProportion * 40.0f));
	for (int i = 0; i < numberNewParticles; i++) {

		float positionOffsetX = MathUtils::RandomBetween(-0.6, 0.6);
		float speedOffset = 10.0f * abs(positionOffsetX) / 0.75;
		
		// Tornar isso MathUtils::RandomBetween(-3.0f, -2.0f), 0.0f, 1.0f) algo mais customizavel, pois depende da nave. 
		// Parametrizar em fun��o da dist�ncia do motor

		vec3 positionVec = vec3(rotationMatrix * vec4(positionOffsetX, MathUtils::RandomBetween(-3.0f, -2.0f), 0.0f, 1.0f));
		ParticlePosition[NextParticleIndex] = vec4(vec4(Controller->GetPosition() + positionVec, 1.0f));

		vec3 speedDir = positionVec;
		speedDir = speedDir / vec3(MathUtils::Norm(speedDir)) * (0.25f + (thrustProportion / 4.0f));
		vec4 speed = vec4(speedDir + this->Controller->GetLinearSpeed() - vec3(rotationMatrix * (vec4(MathUtils::RandomBetween(-5.0f, 5.0f) * (1.0f - thrustProportion/5), (thrustProportion * (10.0f + speedOffset * 5.0f)) + MathUtils::RandomBetween(0.0f, 5.0f), 0.f, 1.0f))), 1.0f);

		float whiteness = MathUtils::RandomBetween(0.60f, 0.80f);
		ParticleColor[NextParticleIndex] = vec4(MathUtils::RandomBetween(0.85f, 1.0f), MathUtils::RandomBetween(0.85f, 1.0f), whiteness, 1.0f);
		// Debug Only -> ParticleColor2[NextParticleIndex] = vec4(0.0f, 1.0f, 0.0f, 1.0f);
		ParticleVelocity[NextParticleIndex] = speed;
		ParticleTTL[NextParticleIndex] = MaxTimeToLive + MathUtils::RandomBetween(0.0f, 0.10f);

		// Atualiza o �ndice circular
		NextParticleIndex = (NextParticleIndex + 1) % MaxParticlesNumber;
		createdParticles++;
		if (ParticleNumber < MaxParticlesNumber) ParticleNumber++;
	}

	if (createdParticles == 0) return;

	glBindVertexArray(ParticlesVaoID);

	int particlesToUpload = createdParticles;
	int overflow = (oldSize + createdParticles) - MaxParticlesNumber;

	if (overflow > 0) {
		// Evitar overflow de upload de part�culas
		particlesToUpload = createdParticles - overflow; 

		glBindBuffer(GL_ARRAY_BUFFER, ParticlesVboID[0]);
		glBufferSubData(GL_ARRAY_BUFFER, oldSize * sizeof(vec4), particlesToUpload * sizeof(vec4), &ParticlePosition[oldSize]);

		glBindBuffer(GL_ARRAY_BUFFER, ParticlesVboID[1]);
		glBufferSubData(GL_ARRAY_BUFFER, oldSize * sizeof(vec4), particlesToUpload * sizeof(vec4), &ParticleVelocity[oldSize]);

		glBindBuffer(GL_ARRAY_BUFFER, ParticlesVboID[2]);
		glBufferSubData(GL_ARRAY_BUFFER, oldSize * sizeof(vec4), particlesToUpload * sizeof(vec4), &ParticleColor[oldSize]);

		glBindBuffer(GL_ARRAY_BUFFER, ParticlesVboID[3]);
		glBufferSubData(GL_ARRAY_BUFFER, oldSize * sizeof(float), particlesToUpload * sizeof(float), &ParticleTTL[oldSize]);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ParticlesVboID[4]);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, oldSize * sizeof(int), particlesToUpload * sizeof(int), &ParticleIndex[oldSize]);

		// Upload do overflow
		glBindBuffer(GL_ARRAY_BUFFER, ParticlesVboID[0]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, overflow * sizeof(vec4), &ParticlePosition[0]);

		glBindBuffer(GL_ARRAY_BUFFER, ParticlesVboID[1]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, overflow * sizeof(vec4), &ParticleVelocity[0]);

		glBindBuffer(GL_ARRAY_BUFFER, ParticlesVboID[2]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, overflow * sizeof(vec4), &ParticleColor[0]); // To debug use &ParticleColor2[0]

		glBindBuffer(GL_ARRAY_BUFFER, ParticlesVboID[3]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, overflow * sizeof(float), &ParticleTTL[0]);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ParticlesVboID[4]);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, overflow * sizeof(int), &ParticleIndex[0]);
	}
	else {
		// Cen�rio onde n�o ocorre overflow
		glBindBuffer(GL_ARRAY_BUFFER, ParticlesVboID[0]);
		glBufferSubData(GL_ARRAY_BUFFER, oldSize * sizeof(vec4), particlesToUpload * sizeof(vec4), &ParticlePosition[oldSize]);

		glBindBuffer(GL_ARRAY_BUFFER, ParticlesVboID[1]);
		glBufferSubData(GL_ARRAY_BUFFER, oldSize * sizeof(vec4), particlesToUpload * sizeof(vec4), &ParticleVelocity[oldSize]);

		glBindBuffer(GL_ARRAY_BUFFER, ParticlesVboID[2]);
		glBufferSubData(GL_ARRAY_BUFFER, oldSize * sizeof(vec4), particlesToUpload * sizeof(vec4), &ParticleColor[oldSize]);

		glBindBuffer(GL_ARRAY_BUFFER, ParticlesVboID[3]);
		glBufferSubData(GL_ARRAY_BUFFER, oldSize * sizeof(float), particlesToUpload * sizeof(float), &ParticleTTL[oldSize]);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ParticlesVboID[4]);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, oldSize * sizeof(int), particlesToUpload * sizeof(int), &ParticleIndex[oldSize]);
	}

	glBindVertexArray(0);
}

ShipModel::~ShipModel()
{
	this->window = nullptr;
	this->Controller = nullptr;
	this->AxisShader = nullptr;
	this->ShipModelShader = nullptr;
	this->ParticleUpdateShader = nullptr;
	this->ParticleShader = nullptr;

	// Liberar buffers de part�culas (VBOs)
	if (ParticlesVboID != nullptr) {
		glDeleteBuffers(5, ParticlesVboID);
		delete[] ParticlesVboID; // Desaloca o array de VBOs
	}

	// Liberar VAO de part�culas
	if (ParticlesVaoID != 0) glDeleteVertexArrays(1, &ParticlesVaoID);

	// Liberar mem�ria das part�culas
	if (ParticlePosition != nullptr) delete[] ParticlePosition;
	if (ParticleVelocity != nullptr) delete[] ParticleVelocity;
	if (ParticleColor != nullptr) delete[] ParticleColor;
	if (ParticleTTL != nullptr) delete[] ParticleTTL;
	if (ParticleIndex != nullptr) delete[] ParticleIndex;

	// AS naves compartilham o shader, isso precisa ser resolvido no shader manager.

	/*Shader* AxisShader;
	Shader* ShipModelShader;
	Shader* ParticleUpdateShader;
	Shader* ParticleShader;
	GLuint* ParticlesVboID;*/

	/*if (ParticleShader != nullptr) {
		delete ParticleShader;
	}

	if (ParticleUpdateShader != nullptr) {
		delete ParticleUpdateShader;
	}

	if (ShipModelShader != nullptr) {
		delete ShipModelShader;
	}*/

	// Limpeza adicional, se necess�rio para outras partes do sistema
}