#include "DevouringSingularityModel.h"
#include "../../scripts/skills/void/DevouringSingularity.h"

using namespace std;

DevouringSingularityModel::DevouringSingularityModel(DevouringSingularity* Skill)
{
	this->Skill = Skill;
	this->Position = Skill->GetAttractingCenter() - CameraManager::GetInstance()->WorldPivot();
	this->Position.z = 0.0f;
} 

void DevouringSingularityModel::Init()
{
	/// matrices setup
	modelMatrix = mat4(); // identity

	SkillShader = ShaderManager::GetInstance()->GetShader(SkillID::DEVOURING_SINGULARITY);
	if (SkillShader == nullptr)
	{
		SkillShader = new Shader();

		printf("\n COMPILANDO SHADERS (1).");
		SkillShader->Compile("shader/skills/singularity.vert", "shader/skills/singularity.frag"); 

		bool res = ObjectLoader::LoadObject(GlobalPaths::DEVOURING_SINGULARITY_MODEL_PATH.c_str(), SkillShader->Vertexes, SkillShader->Uvs, SkillShader->Normals, SkillShader->Indexes);

		if (res)
			printf("Arquivo de modelo 3d carregado com sucesso!");
		else
			printf("Arquivo de modelo 3d não foi carregado com sucesso.");

		ShaderManager::GetInstance()->AddShader(SkillID::DEVOURING_SINGULARITY, SkillShader);
		SkillShader->GenerateSkillBuffers();
	}
	
}

void DevouringSingularityModel::Render()
{
	this->Position = Skill->GetAttractingCenter() - CameraManager::GetInstance()->WorldPivot();
	SkillShader->setUniform(string("MVP"), CameraManager::GetInstance()->MVP()); //ModelViewProjection
	SkillShader->setUniform(string("Time"), (float)this->Skill->GetTimeToLive());
	SkillShader->setUniform(string("Position"), this->Position);
	SkillShader->Render();
}

void DevouringSingularityModel::Update(double deltaTime)
{
	this->GenerateParticles();
}

void DevouringSingularityModel::GenerateParticles()
{

}
