
#include "Particle.h"
using namespace std;

Particle::Particle()
{
	mass = 1;
	forces = std::vector<Force*>();
}

Particle::Particle(GLFWwindow* window, int size)
{
	mass = 1;
	size = size;
	forces = std::vector<Force*>();
}

void Particle::Update(double t)
{
}

void Particle::init()
{

}

