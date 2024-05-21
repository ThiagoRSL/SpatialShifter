#include "Force.h"

Force::Force()
{
	force = glm::vec3(0.0f, 0.0f, 0.0f);
	target = glm::vec3(0.0f, 0.0f, 0.0f);
}

void Force::SetSource(int source) { this->source = source; }
void Force::SetAsVec(glm::vec3 v) { this->force = glm::vec3(v.x, v.y, v.z); }
void Force::SetAsVec(float x, float y, float z) { this->force = glm::vec3(x, y, z); }
