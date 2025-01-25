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
		ShaderManager::GetInstance()->AddShader(SkillID::DEVOURING_SINGULARITY, SkillShader);
	}
	
	bool res = ObjectLoader::LoadObject(GlobalPaths::DEVOURING_SINGULARITY_MODEL_PATH.c_str(), Vertexes, Uvs, Normals, Indexes);

	if (res)
		printf("Arquivo de modelo 3d carregado com sucesso!");
	else
		printf("Arquivo de modelo 3d não foi carregado com sucesso.");

	glGenVertexArrays(1, &VaoID);
	glBindVertexArray(VaoID);
	VboID = new GLuint[3];
	glGenBuffers(3, VboID);
	glBindVertexArray(0);

	GenerateBuffers();
}

void DevouringSingularityModel::Render()
{
	this->Position = Skill->GetAttractingCenter() - CameraManager::GetInstance()->WorldPivot();
	SkillShader->setUniform(string("MVP"), CameraManager::GetInstance()->MVP()); //ModelViewProjection
	SkillShader->setUniform(string("Time"), (float) this->Skill->GetTimeToLive());
	SkillShader->setUniform(string("Position"), this->Position);

	SkillShader->Use();
	glBindVertexArray(VaoID);
	glDrawArrays(GL_TRIANGLES, 0, Indexes.size());
	glBindVertexArray(0);
	glUseProgram(0);
}

void DevouringSingularityModel::Update(double deltaTime)
{
	this->GenerateParticles();
}

void DevouringSingularityModel::GenerateBuffers()
{
	// Definindo as cores dos vértices.
	for (int i = 0; i <= Indexes.size(); i++)
	{
		Colors.push_back(GlobalColors::DEVOURING_SINGULARITY_COLOR);
	}	

	glBindVertexArray(VaoID);

	//Habilita transparência
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glBindBuffer(GL_ARRAY_BUFFER, VboID[0]);
	glBufferData(GL_ARRAY_BUFFER, Vertexes.size() * sizeof(vec3), (GLvoid*)&Vertexes[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
	glEnableVertexAttribArray(0);  // VertexPosition -> layout 0 in the VS
	 
	/*glBindBuffer(GL_ARRAY_BUFFER, VboID[1]);
	glBufferData(GL_ARRAY_BUFFER, Uvs.size() * sizeof(vec2), (GLvoid*)&Uvs[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
	glEnableVertexAttribArray(1);  // UVs -> layout 1 in the VS

	glBindBuffer(GL_ARRAY_BUFFER, VboID[2]);
	glBufferData(GL_ARRAY_BUFFER, Normals.size() * sizeof(vec3), (GLvoid*)&Normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)2, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
	glEnableVertexAttribArray(2);  // Normals -> layout 2 in the VS*/

	glBindBuffer(GL_ARRAY_BUFFER, VboID[1]);
	glBufferData(GL_ARRAY_BUFFER, Colors.size() * sizeof(vec4), (GLvoid*)&Colors[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 4, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
	glEnableVertexAttribArray(1);  // Colors -> layout 3 in the VS

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboID[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indexes.size() * sizeof(int), (GLvoid*)&Indexes[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}


void DevouringSingularityModel::GenerateParticles()
{

}
