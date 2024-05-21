
#include "UpdateManager.h"
using namespace std;

UpdateManager* UpdateManager::updateManagerInstance = nullptr;
double UpdateManager::FIXED_DELTA_TIME = 0.01;	
int UpdateManager::MAX_FPS = 60;
double UpdateManager::FRAME_TIME = 1 / (double) UpdateManager::MAX_FPS;



UpdateManager::UpdateManager()
{
	this->lastFrameTime = 0.0;
	this->frame = 0;
}

void UpdateManager::UpdateElements(double deltaTime)
{
	timeCounter += (float) deltaTime;
	lastDeltaTime = deltaTime;

	//#pragma omp parallel for 
	for (int i = 0; i < UpdateList.size(); i++)
	{
		//UpdateList.at(i)->Update(FIXED_DELTA_TIME);
		UpdateList.at(i)->Update(deltaTime);
	}

	//remover para classe especializada
	for (int i = 0; i < SkillList.size(); i++)
	{
		//SkillList.at(i)->Update(FIXED_DELTA_TIME);
		SkillList.at(i)->Update(deltaTime);
		if (SkillList.at(i)->WornOut())
		{
			//Adicionar pra remover
			//SkillsToEnd.push_back(SkillList.at(i));
		}
	}

}


