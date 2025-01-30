
#include "Projectile.h"
#include "../Ship.h"
using namespace std;

Projectile::~Projectile()
{
	delete this->Collider; // TODO: delete do collider.
	this->source = nullptr; // Não é responsável pelo gerenciamento da memória da fonte.
}

Projectile::Projectile(Entity* source, float scale, float mass, float damage, DamageType damageType, float hitPoints,
	vec3 position, vec3 linearSpeed, Force f, ProjectileTypeID projectileTypeId, vec3 rotation, Faction fact)
{

	this->isDestroyed = false;
	this->ProjectileType = projectileTypeId;
	this->source = source;
	destroyedTime = 0.0f;
	scaleMatrix = glm::scale(glm::mat4(1.0f), vec3(scale, scale, scale));

	rotationMatrix = mat4(); // identity

	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.z), vec3(0.0f, 0.0f, 1.0f));


	this->SetHitPoints(hitPoints);
	this->SetDamage(damage, damageType);
	this->SetMass(mass);
	this->SetPosition(position);
	this->SetLinearSpeed(linearSpeed);
	this->SetFaction(fact);
	this->inertiaMoment = mass; //Todo: Checar isso aqui

	//Aplica o recoil na nave e a força na próprio projétil
	this->AddInstantaneousForce(f);
	f.SetAsVec(-f.GetVec());
	source->AddInstantaneousForce(f);
	this->InitCollider();


	this->Model = ProjectileModel(this);
	this->Model.Init();
	UpdateManager::GetInstance()->AddUpdatable(this);
	RenderManager::GetInstance()->AddRenderable(this);
	CollisionManager::GetInstance()->AddProjectileCollider(this->Collider); //Otimizar.
}

void Projectile::Update(double t)
{
	Entity::Update(t);

	this->Model.Update(t);
	this->Collider->Update(t);

	if (isDestroyed) this->destroyedTime += t;
	if (destroyedTime > 3.0f) Erase();
}

DamageCallback Projectile::ReceiveDamage(float damage, DamageType damageType)
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

void Projectile::CollideAtProjection(Entity& otherEntity, glm::vec3 collisionPoint)
{
	otherEntity.CollideWithProjectile(*this, collisionPoint);
}
void Projectile::CollideWith(Entity& otherEntity, glm::vec3 collisionPoint)
{
	if (source == &otherEntity) return;

	otherEntity.CollideWithProjectile(*this, collisionPoint);
}
void Projectile::CollideWithShip(Ship& otherShip, glm::vec3 collisionPoint)
{
	if (source == &otherShip) return;

	SpacePhysics::EntityCollision(*this, otherShip, collisionPoint, UpdateManager::GetInstance()->GetLastDeltaTime());

	otherShip.ReceiveDamage(this->damage, damageType);
	this->Destroy();
}
void Projectile::CollideWithProjectile(Projectile& otherProjectile, glm::vec3 collisionPoint)
{
	if (source == otherProjectile.GetSource()) return;

	otherProjectile.Destroy();
	this->Destroy();
}

void Projectile::Destroy()
{
	CollisionManager::GetInstance()->RemoveProjectileCollider(this->Collider);
	//TODO: Desocupa espaço da memória sei lá. Fazer depois pra nao crashar

	Entity::Destroy();
}

void Projectile::Erase()
{
	UpdateManager::GetInstance()->RemoveUpdatable(this);
	RenderManager::GetInstance()->RemoveRenderable(this);
}

void Projectile::Render()
{
	this->Model.Render();
}

void Projectile::Init()
{
	return;
}

void Projectile::InitCollider()
{
	Collider = new PolyCollider();
	Collider->Init(this, ShipBuilder::GetInstance()->GetProjectileColliderPath(ProjectileType));
}