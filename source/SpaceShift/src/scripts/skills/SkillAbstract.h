#pragma once

#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "GLUtils.h"

#include <vector>
#include <iostream>

#include "TextureManager.h"

#include "../../common/Shader.h"
#include "../../entities/Entity.h"
#include "../../physics/Force.h"
#include "../../manager/CameraManager.h"
#include "../../Utils/MathUtils.h"
#include "../../Utils/GlobalVars.h"

enum class SkillID {
	DEVOURING_SINGULARITY,
};

enum class SkillState{
	ACTIVE,
	ENDED
};

class SkillAbstract
{
	protected:
		//Conjurador, entidade responsável por invocar a habilidade.
		Entity* source;
		double timeToLive;
		SkillState State;

	public:
		SkillAbstract() { source = NULL; }
		virtual void Init() = 0;
		virtual void Update(double t) = 0;
		virtual void Render() = 0;
		virtual void End() = 0;

		bool WornOut()
		{
			if (timeToLive <= 0.0) return true;
			else return false;
		}

};