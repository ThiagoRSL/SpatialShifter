
#include "StageManager.h"
using namespace std;

StageManager* StageManager::StageManagerInstance = nullptr;

StageManager::StageManager()
{
	Init();
	BossSpawned = false;
}

void StageManager::UpdateStage(double t)
{
	int deltaDifficulty = KillCounter / 2;
	for (int i = EnemyCounter; i < (10 + deltaDifficulty); i++)
	{
		SpawnEnemy();
	}
	if (KillCounter >= 10 && BossSpawned == false)
	{
		SpawnBoss(); 
		BossSpawned = true;
	}

}

void StageManager::SpawnEnemy()
{
	float r1 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 100));
	float r2 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 100));
	vec3 proposedPosition = vec3((r2 - 50.0f), (r1 - 50.0f), 0.0f);

	Ship* tempEnemyShip;
	tempEnemyShip = new Ship(ShipTypeId::NANO_SHIP_ID);

	if (MathUtils::Norm(proposedPosition - PlayerShip->GetPosition()) > 5.0f)
		tempEnemyShip->SetPosition(proposedPosition);
	else
	{
		proposedPosition.x += 5.0f;
		proposedPosition.y -= 5.0f;
		tempEnemyShip->SetPosition(proposedPosition);
	}
	tempEnemyShip->SetShipColor(vec4(0.70f, 0.30f, 0.30f, 1.0f));
	tempEnemyShip->SetMass(500);
	tempEnemyShip->SetScale(vec3(0.5f, 0.5f, 0.5f));
	tempEnemyShip->SetHitPoints(20.0f);
	tempEnemyShip->SetCollidingRaidius(15.f);
	tempEnemyShip->SetFaction(Faction::PIRATES);
	tempEnemyShip->SetThrustIntensityMax(200000.0f);
	tempEnemyShip->Init();
	if (!GlobalVars::BRAIN_LESS)
		tempEnemyShip->SetAutonomous(new AutonomyShipModule(tempEnemyShip, 100.f, PlayerShip->GetPositionRef()));
	//tempEnemyShip->SetAngularSpeed(tempEnemyShip->GetPosition() + vec3(0.0f, 0.0f, 10.0f));
	Enemies.push_back(tempEnemyShip);
	EnemyCounter++;
}

void StageManager::SpawnBoss()
{
	Ship* tempEnemyShip = new Ship(ShipTypeId::ZWEIHANDER_SHIP_ID);
	tempEnemyShip->SetPosition(vec3(PlayerShip->GetPosition().x + 50.0f, PlayerShip->GetPosition().y+50.0f, 0.0f));
	tempEnemyShip->SetShipColor(vec4(0.75f, 0.10f, 0.10f, 1.0f));
	tempEnemyShip->SetMass(25000);
	tempEnemyShip->SetScale(vec3(.5f, .5f, .5f));
	tempEnemyShip->SetHitPoints(1000.0f);
	tempEnemyShip->SetCollidingRaidius(100.f);
	tempEnemyShip->SetAngularSpeed(vec3(0.0f, 0.0f, 0.0f));
	tempEnemyShip->SetFaction(Faction::RED_CROSS);
	tempEnemyShip->SetThrustIntensityMax(25000000.0f);
	tempEnemyShip->Init();
	if(!GlobalVars::BRAIN_LESS)
		tempEnemyShip->SetAutonomous(new AutonomyShipModule(tempEnemyShip, GlobalVars::STANDARD_AWARE_RADIUS * 10, PlayerShip->GetPositionRef()));
	Enemies.push_back(tempEnemyShip);
}


void StageManager::Init()
{
	KillCounter = 0;
	EnemyCounter = 0;	
	Enemies = vector<Ship*>();
}


