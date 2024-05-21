#include "Ship.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include "TextureManager.h"
#include "common/ObjectLoader.h"
#include "manager/CameraManager.h"
#include "manager/RenderManager.h"
#include "manager/UpdateManager.h"
#include "GLUtils.h"

using namespace std;

class RenderManager;
Ship::Ship(GLFWwindow* window, int size)
{
	this->size = size;
	this->Model = ShipModel(this, window, size);

	//Empuxo
	thrustForce = Force();
	this->AddForce(&thrustForce);

	position = vec3(500.0f, 500.0f, 0.0f);
	rotation = vec3(0.0f, 0.0f, 0.0f);
	direction = vec3(1.0f, 0.0f, 0.0f);

	thrustIntensity = 0.0f;
	thrustIntensityDelta = 25000.0f;
	thrustIntensityMax = 300000.0f;

	rotateDelta = 2.5f;

	direction = vec3(1.0f, 0.0f, 0.0f);
	speedMax = 50.0f;
}

void Ship::Init()
{
	this->Model.Init();	
	RenderManager::GetInstance()->AddRenderable(this);
	UpdateManager::GetInstance()->AddUpdatable(this);
}

void Ship::ShotAt(vec3 shotTarget)
{

}

//ajusta saporra
void Ship::RotateDirection(vec3 rotation)
{
	direction.x = 1.0 * glm::cos(glm::radians(rotation.z + 90));
	direction.y = 1.0 * glm::sin(glm::radians(rotation.z + 90));
}

void Ship::AdjustRotation(vec3 relativeAdjust)
{
	this->ChangeRotation(vec3(relativeAdjust.x * rotateDelta, relativeAdjust.y * rotateDelta, relativeAdjust.z * rotateDelta));
}

void Ship::ChangeRotation(vec3 rotateVec)
{
	//printf("\nRotação do Jogador (%f º,%f º,%f º).", rotation.x, rotation.y, rotation.z);
	rotation += rotateVec;
	if (rotation.x >= 360.0)	rotation.x = 0.0;
	else if (rotation.x < 0.0)	rotation.x = 360.0;
	if (rotation.y >= 360.0)	rotation.y = 0.0;
	else if (rotation.y < 0.0)	rotation.y = 360.0;
	if (rotation.z >= 360.0)	rotation.z = 0.0;
	else if (rotation.z < 0.0)	rotation.z = 360.0;
	RotateDirection(rotation);
}

void Ship::AdjustThrust(float value, double deltaTime)
{
	double delta = value * deltaTime * thrustIntensityMax;
	this->CalculateThrust(delta);
}
void Ship::CalculateThrust(float thrustIntensityDelta)
{
	thrustIntensity += thrustIntensityDelta;
	if (thrustIntensity > thrustIntensityMax)
		thrustIntensity = thrustIntensityMax;
	if (thrustIntensity < 0.0f)
		thrustIntensity = 0.0f;
}
void Ship::UpdateThrust(double deltaTime)
{
	vec3* tf = thrustForce.GetVecRef();
	*tf = vec3(vec3(thrustIntensity) * direction);
}
void Ship::Update(double deltaTime)
{
	this->Model.Update(deltaTime);
	UpdateThrust(deltaTime);
	Entity::Update(deltaTime);
}

