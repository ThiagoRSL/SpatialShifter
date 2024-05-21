#pragma once

#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <vector>

#include "../../common/Shader.h"
#include "../../entities/Entity.h"
#include "../../scripts/skills/SkillAbstract.h"
#include "../RenderModel.h"

class DevouringSingularity;

class DevouringSingularityModel : public RenderModel
{

private:
	vec3 Position;
	DevouringSingularity* Skill;
	Shader* SkillShader;

public:
	DevouringSingularityModel() { ; }
	DevouringSingularityModel(DevouringSingularity* Skill);

	// mesh virtual functions
	void Init();
	void Update(double t);
	void Render();
	void GenerateBuffers();
	void GenerateParticles();
	//void resize(int, int);

};


/*

Uma nave espacial leve pesa entre 40 e 100 toneladas e requer de dez a trinta tripulantes.
Uma nave espacial média pesa entre 100 e 1000 toneladas e requer de vinte a quinhentos tripulantes.
Uma nave espacial pesada pesa mais de 1000 toneladas e requer mais de quinhentos tripulantes para operar.
*/