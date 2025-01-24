
#include "RenderManager.h"
using namespace std;

RenderManager* RenderManager::renderManagerInstance = nullptr;


RenderManager::RenderManager()
{
	CameraManagerInstance = CameraManager::GetInstance(); 
}

void RenderManager::RenderElements()
{
	vec3 CameraDistance = CameraManagerInstance->WorldPivot();
	#pragma omp parallel for 
	for (int i = 0; i < RenderList.size(); i++)
	{
		float distance = MathUtils::Norm(CameraDistance - RenderList.at(i)->GetPosition());
		if (distance < GlobalVars::RENDER_DISTANCE)
		{
			RenderList.at(i)->Render();
		}		
	}
}


