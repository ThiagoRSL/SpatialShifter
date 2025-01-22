#pragma once

#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "GLUtils.h"

#include <vector>
#include <iostream>

#include "TextureManager.h"

#include "../SkillAbstract.h"
#include "../../../common/Shader.h"
#include "../../../entities/Entity.h"
#include "../../../physics/Force.h"
#include "../../../manager/CameraManager.h"
#include "../../../manager/CollisionManager.h"
#include "../../../manager/ShaderManager.h"
#include "../../../Utils/MathUtils.h"
#include "../../../Utils/GlobalVars.h"
#include "../../../models/skills/DevouringSingularityModel.h"


class DevouringSingularity : public SkillAbstract
{
	private:
		void UpdateAttractingEntities(double deltaTime);

		int skillId;
		float SingularityMass;
		float AttractingRadius;
		double BaseDamagePerSecond;
		vec3 AttractingCenter;
		vec4 singularityColor;
		std::map<Entity*, Force*> DictAttractingEntities;
		DevouringSingularityModel Model;

	public:
		DevouringSingularity(Entity* sourceEntity, vec3 targetpoint, float mass, float dps, double duration);
		void Init();
		void Update(double t);
		void Render() { this->Model.Render(); }
		void End();

		void PrepareShader();
		const vec3 GetAttractingCenter() { return this->AttractingCenter; }
		const double GetTimeToLive() { return this->timeToLive; }
};