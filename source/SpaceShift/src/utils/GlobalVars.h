#pragma once

#include "glm/glm.hpp"
#include <vector>
#include <iostream>

class GlobalColors
{
public:
	static glm::vec4 DEVOURING_SINGULARITY_COLOR;
};

class GlobalPaths
{
public:
	static std::string DEVOURING_SINGULARITY_MODEL_PATH;
	static std::string BACKGROUND_IMAGE_PATH;
	static std::string CURSOR_IMAGE_PATH;
	static std::string HOTBAR_EMPTY_SLOT_IMAGE_PATH;
	static std::string PARTICLE_ENGINE_PATH;
	

	
};

class GlobalVars
{
	public:
		static float STANDARD_COLLIDING_RADIUS;
		static float RENDER_DISTANCE;
		static float STANDARD_AWARE_RADIUS;
		static bool PERSPECTIVE;
		static bool FULL_SCREEN;
		static bool DEBUG_MODE;
		static bool SAME_FACTION_COLLIDE;
		static bool COLLIDE_AFTER_DESTRUCTION;
		static bool COLLIDING_CAUSES_DAMAGE;
		static bool ALPHA_PHYSICS;
		static bool BRAIN_LESS;

	public:
};

enum class DamageType
{ 
	COLLISION_DAMAGE,
	BALISTIC_DAMAGE,
	LASER_DAMAGE,
	ELETROMAGNETIC_DAMAGE,
	FORCE_DAMAGE,
	EXPLOSION_DAMAGE,
	VOID_DAMAGE,
};

enum class SkillType {
	DEVOURING_SINGULARITY_ID
};

enum class ShipTypeId {
	STANDARD_SHIP_ID,
	MICRO_SHIP_ID,
	NANO_SHIP_ID,
	ZWEIHANDER_SHIP_ID
};

enum class ShipState {
	STANDARD,
	DESTROYED,
};

enum class Faction {
	FACTION_LESS,
	PLAYER,
	PIRATES,
	RED_CROSS,
};

enum class ProjectileTypeID {
	BALLISTIC_PROJECTILE_ID,
};