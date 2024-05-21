//**********************************************
//Singleton Text Manager class
//Written by Ben English
//benjamin.english@oit.edu
//
//For use with OpenGL and the FreeImage library
//**********************************************

#ifndef TextManager_H
#define TextManager_H

#include <windows.h>
#include "GL/glew.h"
#include "Freeimage/FreeImage.h"
#include <ft2build.h>
#include FT_FREETYPE_H  

#include <string>
#include <map>
#include <glm\glm.hpp>

#include "ShaderManager.h"
#include "CameraManager.h"
#include "../common/Shader.h"

using std::string;

class InterfaceManager;

class TextElement
{
public:

	glm::vec2 Position;
	std::string Text;
	glm::vec4 TextColor;
	float Scale;
	float LineSpacing;
	float LineWidthMax;
	float MarginLeft;
	float MarginTop;

public:
	TextElement()
	{
		Position = vec2(0.0f, 0.0f);
		Text = "";
		TextColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
		Scale = 1.0f;
		LineSpacing = 1.0f;
		LineWidthMax = 1000.0f;
		MarginLeft = 0.0f;
		MarginTop = 0.0f;
	}
	void SetMargins(float ml, float mt) { this->MarginLeft = ml; this->MarginTop = mt; }
};

struct TextCharacter {
	unsigned int TextureID;  // ID handle of the glyph texture
	glm::ivec2   Size;       // Size of glyph
	glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
	unsigned int Advance;    // Offset to advance to next glyph
};

class TextManager
{
private:
	GLuint VaoID;
	GLuint* VboID;

	void Init();
	TextManager();
	static TextManager* textManagerInstance;
	std::map<char, TextCharacter> TextCharacters;
	Shader* TextShader;

public:
	static TextManager* GetInstance();
	virtual ~TextManager();

	glm::mat4* InterfaceProjection;	//Referencia à matriz associada ao Interface Manager.
	glm::vec3 size;

	//void RenderText(const TextElement& te); 
	void RenderText(const TextElement* te);
	void SetInterfaceProjection(glm::mat4* projection)
	{
		InterfaceProjection = projection;
		TextShader->setUniform(string("InterfaceProjectionMatrix"), mat4(*InterfaceProjection));
	}



};

#endif