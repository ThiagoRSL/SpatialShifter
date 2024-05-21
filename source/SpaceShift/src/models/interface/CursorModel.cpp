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
	}

	// Get a TextureManager's instance
	TextureManager* texManager = TextureManager::Inst();

	// TODO: Melhor uso das unidades de Textura
	glActiveTexture(GL_TEXTURE0);

	TextureIndex = TextureManager::Inst()->ReserveIndex();
	if (!texManager->LoadTexture(GlobalPaths::CURSOR_IMAGE_PATH.c_str(), TextureIndex))
		cout << "Failed to load texture." << endl;


	glGenVertexArrays(1, &VaoID);
	glBindVertexArray(VaoID);
	VboID = new GLuint[3];
	glGenBuffers(3, VboID);
	glBindVertexArray(0);

	GenerateBuffers();
}

void CursorModel::Render()
{
	CursorShader->setUniform(std::string("CursorPosition"), mousePosition);
	//printf("\n\n X: %f, Y: %f", mousePosition.x, mousePosition.y);
	glActiveTexture(GL_TEXTURE0);
	TextureManager::Inst()->BindTexture(TextureIndex);

	CursorShader->Use();
	glBindVertexArray(VaoID);
	glDrawElements(GL_TRIANGLE_STRIP, Indexes.size(), GL_UNSIGNED_INT, (GLubyte*)NULL);
	glBindVertexArray(0);

}

void CursorModel::Update(double deltaTime)
{
	CursorShader->setUniform(std::string("CursorPosition"), mousePosition);
}

void CursorModel::Resize()
{
	GenerateBuffers();
}

void CursorModel::GenerateBuffers()
{
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	//Os dois por height por que quero um quadrado
	vec2 screenDelta = vec2(48.0f / InputManager::GetInstance()->GetWindowSize().x, 48.0f / InputManager::GetInstance()->GetWindowSize().y);
	

	Vertexes.clear();
	Uvs.clear();
	Indexes.clear();

	Vertexes.push_back(vec3(-screenDelta.x, -screenDelta.y, 0.0f));
	Vertexes.push_back(vec3(-screenDelta.x, screenDelta.y, 0.0f));
	Vertexes.push_back(vec3(screenDelta.x, -screenDelta.y, 0.0f));
	Vertexes.push_back(vec3(screenDelta.x, screenDelta.y, 0.0f));

	Uvs.push_back(vec2(0.0, 0.0));
	Uvs.push_back(vec2(0.0, 1.0));
	Uvs.push_back(vec2(1.0, 0.0));
	Uvs.push_back(vec2(1.0, 1.0));

	Indexes.push_back(0);
	Indexes.push_back(1);
	Indexes.push_back(2);
	Indexes.push_back(3);

	glBindVertexArray(VaoID);

	//Habilita transparência
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glBindBuffer(GL_ARRAY_BUFFER, VboID[0]);
	glBufferData(GL_ARRAY_BUFFER, Vertexes.size() * sizeof(vec3), (GLvoid*)&Vertexes[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
	glEnableVertexAttribArray(0);  // VertexPosition -> layout 0 in the VS

	glBindBuffer(GL_ARRAY_BUFFER, VboID[1]);
	glBufferData(GL_ARRAY_BUFFER, Uvs.size() * sizeof(vec2), (GLvoid*)&Uvs[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
	glEnableVertexAttribArray(1);  // TexCoord -> layout 2 in the VS

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboID[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indexes.size() * sizeof(int), (GLvoid*)&Indexes[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
	CursorShader->setUniform("TextureSampler", 0);
}