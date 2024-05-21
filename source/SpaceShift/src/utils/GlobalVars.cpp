#include "GlobalVars.h"


bool  GlobalVars::ALPHA_PHYSICS = true;
bool GlobalVars::PERSPECTIVE = false;
float GlobalVars::RENDER_DISTANCE = 500.0f;
bool  GlobalVars::FULL_SCREEN = true;
bool  GlobalVars::DEBUG_MODE = false;
bool  GlobalVars::BRAIN_LESS = true;

// Collision
bool GlobalVars::SAME_FACTION_COLLIDE = true;
bool  GlobalVars::COLLIDING_CAUSES_DAMAGE = true;
bool GlobalVars::COLLIDE_AFTER_DESTRUCTION = true;
float GlobalVars::STANDARD_COLLIDING_RADIUS = 25.0f;
float GlobalVars::STANDARD_AWARE_RADIUS = 100.0f;

std::string GlobalPaths::DEVOURING_SINGULARITY_MODEL_PATH = std::string("..//source//SpaceShift//assets//skills//devouringSphere.obj");

std::string GlobalPaths::BACKGROUND_IMAGE_PATH = std::string("..//source//SpaceShift//assets//background.png");
std::string GlobalPaths::CURSOR_IMAGE_PATH = std::string("..//source//SpaceShift//assets//cursor.png");
std::string GlobalPaths::HOTBAR_EMPTY_SLOT_IMAGE_PATH = std::string("../source/SpaceShift/assets/sprites//EmptySlot.png");

std::string GlobalPaths::PARTICLE_ENGINE_PATH = std::string("..//source//SpaceShift//assets//cursor.png");

glm::vec4 GlobalColors::DEVOURING_SINGULARITY_COLOR = glm::vec4(0.1012f, 0.0118f, 0.38f, 1.0f);
