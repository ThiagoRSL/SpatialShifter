#include "Ship.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "GLUtils.h"
#include <iostream>
#include "TextureManager.h"

#include "../common/ObjectLoader.h"
#include "../manager/CameraManager.h"
#include "../manager/RenderManager.h"
#include "../manager/UpdateManager.h"
#include "../manager/StageManager.h"
#include "../manager/CollisionManager.h"
#include "../builders/ShipBuilder.h"
#include "../scripts/skills/void/DevouringSingularity.h"

using namespace std;

class RenderManager;

Ship::~Ship()
{
	//std::vector<SkillAbstract*> ActiveSkills;
	//std::vector<SkillAbstract*> SkillsToEnd;
	//delete this->Autonomy; // TODO: Achar lugar melhor pra isso (dá erro)
}

Ship::Ship(ShipTypeId shipTypeId)
{
	this->speedMax = 100.0f;
	this->ShipType = shipTypeId;
	this->isAutonomous = false;
	this->Autonomy = NULL;
	this->faction = Faction::FACTION_LESS;
	this->hitPoints = 1;
	this->shotCooldown = 0.0f;
	this->shotCooldownTime = 0.25f;

	this->shipState = ShipState::STANDARD;
	this->destroyedTime = 0.0f;

	this->ActiveSkills = std::vector<SkillAbstract*>();
	this->SkillsToEnd = std::vector<SkillAbstract*>();


	position = vec3(0.0f, 0.0f, 1.0f);
	rotation = vec3(0.0f, 0.0f, 0.0f);
	direction = vec3(1.0f, 0.0f, 0.0f);

	thrustMultiplier = 0.0f;
	thrustIntensity = 0.0f;
	thrustIntensityMax = 1.0f;

	rotateDelta = 2.5f;
	speedMax = 50.0f;

	//Empuxo
	thrustForce = Force();
	this->AddForce(&thrustForce);

	rotationWheelForce = Force();
	rotationWheelForce.SetAsVec(0.0f, 0.0f, 0.0f);
	rotationWheelForce.SetTargetPoint(vec3(0, 15, 0));
	this->AddForce(&rotationWheelForce);

	rotationWheelForceCounter = Force();
	rotationWheelForceCounter.SetAsVec(0.0f, 0.0f, 0.0f);
	rotationWheelForceCounter.SetTargetPoint(vec3(0, -15, 0));
	this->AddForce(&rotationWheelForceCounter);

	rotationWheelXForce = Force();
	rotationWheelXForce.SetAsVec(0.0f, 0.0f, 0.0f);
	rotationWheelXForce.SetTargetPoint(vec3(15, 0, 0));
	this->AddForce(&rotationWheelXForce);

	rotationWheelXForceCounter = Force();
	rotationWheelXForceCounter.SetAsVec(0.0f, 0.0f, 0.0f);
	rotationWheelXForceCounter.SetTargetPoint(vec3(-15, 0, 0));
	this->AddForce(&rotationWheelXForceCounter);

	this->Model = ShipModel(this);
}

void Ship::Init()
{
	this->Model.Init();
	this->Collider.Init(this, ShipBuilder::GetInstance()->GetColliderModelPath(ShipType));

	this->CalculateShipInertia(this->Model.GetVertexes());

	RenderManager::GetInstance()->AddRenderable(this);
	UpdateManager::GetInstance()->AddUpdatable(this);
	CollisionManager::GetInstance()->AddCollider(&this->Collider);
}

void Ship::UseSkill(vec3 shotTarget)
{
	if (this->shipState == ShipState::DESTROYED)
		return;

	DevouringSingularity* ds = new DevouringSingularity(this, shotTarget, 40000.0f, 1.0, 10.0f);
	ActiveSkills.push_back(ds);
	UpdateManager::GetInstance()->AddUpdatable(ds);
}

void Ship::ShotAt(vec3 shotTarget)
{
	if (this->shotCooldown > 0.0f)
		return;
	if (this->shipState == ShipState::DESTROYED)
		return;

	//printf("\nPosition: (%f, %f, %f)", position.x, position.y, position.z);
	//printf("\nTarget: (%f, %f, %f)", shotTarget.x, shotTarget.y, shotTarget.z);

	//Força que da impulso para o projétil e da o recoil na nave.
	shotTarget.z = position.z;
	glm::vec3 dir = glm::normalize(shotTarget - position);

	// calcular o ângulo de rotação no eixo Z
	float angle = - glm::degrees(atan2(dir.x, dir.y));

	Force f = Force();
	f.SetAsVec(dir * 125000.0f);


	Projectile* p = new Projectile(this, 0.4f, 15.0f, 10.0f, DamageType::BALISTIC_DAMAGE, 5.0f, this->position,
		this->linearSpeed, f, ProjectileTypeID::BALLISTIC_PROJECTILE_ID, vec3(0.0f, 0.0f, angle), this->faction);


	this->shotCooldown = this->shotCooldownTime;
}


void Ship::AdjustRotationX(float WheelForceVal)
{
	if (this->shipState == ShipState::DESTROYED)
		return;
	this->rotationWheelXForce.SetAsVec(0.0f, 0.0f, 1000000.0f * WheelForceVal);
	this->rotationWheelXForceCounter.SetAsVec(0.0f, 0.0f, -1000000.0f * WheelForceVal);
}

void Ship::AdjustRotationZ(float WheelForceVal)
{
	if (this->shipState == ShipState::DESTROYED)
		return;
	this->rotationWheelForce.SetAsVec(1000000.0f * WheelForceVal, 0.0f, 0.0f);
	this->rotationWheelForceCounter.SetAsVec(-1000000.0f * WheelForceVal, 0.0f, 0.0f);
	//this->ChangeRotation(vec3(relativeAdjust.x * rotateDelta, relativeAdjust.y * rotateDelta, relativeAdjust.z * rotateDelta));
}

void Ship::UpdateRotation()
{
	RotateDirection();
}

void Ship::SetThrustMode(float value)
{
	thrustMultiplier = value;
}
void Ship::CalculateThrust(double deltaTime)
{
	thrustIntensity += thrustMultiplier * deltaTime * thrustIntensityMax;
	if (thrustIntensity > thrustIntensityMax)
		thrustIntensity = thrustIntensityMax;
	if (thrustIntensity < 0.0f)
		thrustIntensity = 0.0f;
}
void Ship::UpdateThrust(double deltaTime)
{
	if (this->shipState == ShipState::DESTROYED)
		return;

	CalculateThrust(deltaTime);

	vec3* tf = thrustForce.GetVecRef();
	*tf = vec3(vec3(thrustIntensity) * direction);
}
void Ship::Update(double deltaTime)
{
	if (isAutonomous)
	{
		if (this->shipState != ShipState::DESTROYED)
			Autonomy->Update(deltaTime);
	}		

	if(this->shotCooldown > 0.0f)
		this->shotCooldown -= deltaTime;
	//Fazer a destruição das skills settadas pra morrer
	//for (int i = 0; i < SkillsToEnd.size(); i++){}

	if (this->shipState == ShipState::DESTROYED) UpdateDestroyedTime(deltaTime);

	UpdateThrust(deltaTime);
	Entity::Update(deltaTime);
	UpdateRotation();

	this->Model.Update(deltaTime);
	this->Collider.Update(deltaTime);
}

void Ship::UpdateDestroyedTime(double deltaTime)
{
	this->destroyedTime += deltaTime;	
}


void Ship::CollideWith(Entity& otherEntity, glm::vec3 collisionPoint)
{
	otherEntity.CollideWithShip(*this, collisionPoint);
}


void Ship::CollideWithProjectile(Projectile& otherProjectile, glm::vec3 collisionPoint)
{
	otherProjectile.CollideWithShip(*this, collisionPoint);
}

void Ship::CollideAtProjection(Entity& otherEntity, glm::vec3 collisionPoint)
{
	otherEntity.CollideWithShip(*this, collisionPoint);
}

void Ship::CalculateShipInertia(const std::vector<glm::vec3>& vertexes)
{
	float inertia = 0.0f;
	for (int i = 0; i < this->Model.GetVertexes().size(); i++)
	{
		float r = MathUtils::Norm(this->Model.GetVertexes().at(i));
		// Norma do vetor referente ao ponto 0 = raio.
		inertia += r * r * (this->mass / (float)this->Model.GetVertexes().size());
	}
	this->SetInertia(inertia);
}

DamageCallback Ship::ReceiveDamage(float damage, DamageType damageType)
{
	if (this->IsDestroyed()) return DamageCallback::ALREADY_DESTROYED;

	switch (damageType)
	{
		case DamageType::COLLISION_DAMAGE:
			this->hitPoints -= damage;
			printf("\n Dano recebido: %f", damage);
			break;
		case DamageType::VOID_DAMAGE:
			this->hitPoints -= damage;
			break;
		case DamageType::BALISTIC_DAMAGE:
			this->hitPoints -= damage;
			break;
	}

	if (hitPoints <= 0.0f)
	{
		Destroy();
		return DamageCallback::DESTROYED;
	}

	return DamageCallback::RECEIVED;
}

void Ship::Destroy()
{
	this->shipState = ShipState::DESTROYED;
	this->thrustForce = vec3(0.0f);
	this->thrustIntensity = 0.0f;

	this->angularAcceleration = vec3(0.0f);
	this->acceleration = vec3(0.0f);
	this->linearSpeed = vec3(0.0f);

	CollisionManager::GetInstance()->RemoveCollider(&this->Collider);
	StageManager::GetInstance()->ShipDestroyed(this);
	Entity::Destroy();

	// Desaloca a memória da autonomia, deve ser colocado dentro do destrutor mas ele só deve ser chamado após a animação de destruição.
	// Entretanto, por hora, isso fica aqui
	delete this->Autonomy;
	this->Autonomy = nullptr;
}



void Ship::CollideWithShip(Ship& otherShip, glm::vec3 collisionPoint)
{

	//Caso a nave esteja destruida, apenas ignora a colisão.
	if (!GlobalVars::COLLIDE_AFTER_DESTRUCTION && (this->shipState == ShipState::DESTROYED || otherShip.shipState == ShipState::DESTROYED))
		return;

	//Dano é calculado dentro da colisão e repassado para a classe nave.
	SpacePhysics::EntityCollision(*this, otherShip, collisionPoint, UpdateManager::GetInstance()->GetLastDeltaTime());
}

void Ship::SetAutonomy(AutonomyShipModule* ASM)
{
	this->Autonomy = ASM;
	this->SetAutonomous(false);
}

void Ship::SetAutonomous(bool value)
{
	if (this->Autonomy != NULL)
	{
		this->isAutonomous = value;
		if (!value && this->Autonomy != nullptr)
			this->Autonomy->ClearTasks();
	}
	else
	{
		printf("\n Ocorreu uma tentativa de definir como Autonoma uma nave que não possui um módulo de autonomia.");
	}
}
