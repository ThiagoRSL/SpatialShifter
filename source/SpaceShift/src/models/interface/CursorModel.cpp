#include "CursorModel.h"

#include <windows.h>
#include <iostream>
#include <string>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "GLUtils.h"
#include "TextureManager.h"
#include "../../common/ObjectLoader.h"
#include "../../manager/CameraManager.h"
#include "../../manager/InputManager.h"
#include "../../manager/InterfaceManager.h"
#include "../../manager/ShaderManager.h"
#include "../../builders/ShipBuilder.h"
#include "../../entities/Ship.h"

using namespace std;

std::wstring ExePath() {
	TCHAR buffer[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
	return std::wstring(buffer).substr(0, pos);
}

TextureManager* texManager;


void CursorModel::Init()
{
	CursorShader = ShaderManager::GetInstance()->GetCursorShader();
	if (CursorShader == NULL)
	{
		CursorShader = new Shader();
		printf("\n COMPILANDO SHADERS (IE).");
		CursorShader->Compile("shader/interface/cursor.vert", "shader/interface/cursor.frag");
		ShaderManager::GetInstance()->SetCursorShader(CursorShader);
		// TODO: REDEFINIR QUANDO RESIZE
		CursorShader->setUniform(string("InterfaceProjection"), InterfaceManager::GetInstance()->GetInterfaceProjection());

		// Get a TextureManager's instance
		TextureManager* texManager = TextureManager::Inst();

		// TODO: Melhor uso das unidades de Textura
		glActiveTexture(GL_TEXTURE0);

		TextureIndex = TextureManager::Inst()->GetTexture(GlobalPaths::CURSOR_IMAGE_PATH);
		if (TextureIndex == -1) printf("\n Nao foi carregada a textura.");

		CursorShader->GenerateMouseBuffers();
	}

}

void CursorModel::Render()
{
	CursorShader->setUniform(std::string("CursorPosition"), mousePosition);
	//printf("\n\n X: %f, Y: %f", mousePosition.x, mousePosition.y);
	glActiveTexture(GL_TEXTURE0);
	TextureManager::Inst()->BindTexture(TextureIndex);

	CursorShader->RenderMouse();
}

void CursorModel::Update(double deltaTime)
{
	CursorShader->setUniform(std::string("CursorPosition"), mousePosition);
}

void CursorModel::Resize()
{
	CursorShader->GenerateMouseBuffers();
}
