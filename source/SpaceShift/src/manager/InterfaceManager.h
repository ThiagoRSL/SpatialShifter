#pragma once

#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <iostream>
#include <vector>

#include "TextureManager.h"
#include "TextManager.h"
#include "GLUtils.h"
#include "../entities/Entity.h"
#include "../colliders/PolyCollider.h"

class InputManager;

enum class InterfaceInteraction {
	MOUSE_CLICK_LEFT,
	MOUSE_CLICK_RIGHT,
	MOUSE_HOVER,
	MOUSE_LEAVE
};

enum class InterfaceElementIndex {
	INDEX_LESS,
	ROOT,
	INVENTORY,
	QUICK_SLOT_BAR, 
	QS1, QS2, QS3, QS4, QS5, 
	QS6, QS7, QS8, QS9, QS0,
};

enum class ElementPlacement {
	ABSOLUTE_PLACEMENT,
	RELATIVE_PLACEMENT
};

class InterfaceElement
{
private:
	Shader* IEShader;
	InterfaceElementIndex ElementIndex;
	InterfaceElement* FatherElement;
	ElementPlacement Placement;

	std::vector<vec2> Vertexes;
	std::vector<vec2> TexCoord;
	std::vector<int> Indexes;
	std::vector<int> VertexIndexes;
	GLuint* VboID;

	GLuint InterfaceElementVBOID;
	GLuint BackgroundTextureID;
	int TextureIndex;
	bool HasTexture;
	bool DebugHighlight;


	vec4 backgroundColor;
	vec4 borderColor;
	vec2 RelativePosition;
	float height;
	float width;

	std::vector<InterfaceElement*> ChildrenElements;
	std::vector<TextElement*> TextComponents;

public:
	InterfaceElement();
	void AddInterfaceComponent(InterfaceElement* element);
	void AddText(std::string Text);
	void RenderChildren();
	void Render();
	void Resize();
	void Interact(InterfaceInteraction interaction);

	void SetElementIndex(InterfaceElementIndex index);
		
	void SetPlacement(ElementPlacement p) { this->Placement = p; }
	void SetFather(InterfaceElement* father)
	{
		this->FatherElement = father;
		this->UpdateComponents();
	}
	void SetBackgroundColor(vec4 c) { this->backgroundColor = c; }
	void SetBorderColor(vec4 c) { this->borderColor = c; }
	void SetPosition(vec2 p) { this->RelativePosition = p; }
	void SetHeight(float h) { this->height = h; this->Resize(); }
	void SetWidth(float w) { this->width = w; this->Resize(); }

	float GetHeight() { return height; }
	float GetWidth() { return width; }
	vec4 GetBackgroundColor() { return backgroundColor; }
	ElementPlacement GetPlacement() { return Placement; }
	std::vector<InterfaceElement*>* GetChildren();
	vec2 GetPosition();

	void SetTexture(std::string NewTexture);
	void GenerateBuffers();
	void UpdateComponents();

	void DebugHighlighted()
	{
		this->DebugHighlight = true;
	}

};

class InterfaceManager
{
private:
	static InterfaceManager* interfaceManager;
	GLFWwindow* window = nullptr;
	InterfaceManager();

	std::map<InterfaceElementIndex, InterfaceElement*> DictIndexedInterfaceElements;
	std::vector<InterfaceElement*> RenderList;
	InterfaceElement* RootInterfaceElement;
	Shader* InterfaceShader;
	mat4 InterfaceProjection;


	InterfaceElement* MouseHoverElement = NULL;


public:
	mat4 GetInterfaceProjection() { return this->InterfaceProjection; }
	mat4* GetInterfaceProjectionRef() { return &this->InterfaceProjection; }
	void SetInterfaceProjection(mat4 projectionMatrix) { this->InterfaceProjection = projectionMatrix; }

	void SetMouseHoverElement(InterfaceElement* IE) { MouseHoverElement = IE; };
	InterfaceElement* GetMouseHoverElement() { return MouseHoverElement; }

	void Init();
	void SetElementIndexed(InterfaceElement* element, InterfaceElementIndex index) { DictIndexedInterfaceElements[index] = element; }
	void AddInterfaceComponent(InterfaceElement* element, InterfaceElementIndex index);
	void RenderInterface();
	void RenderElement(InterfaceElementIndex index);
	void SetToRender(InterfaceElementIndex index);
	void UnsetToRender(InterfaceElementIndex index);
	void ToggleRender(InterfaceElementIndex index);
	InterfaceElement* GetIndexedElement(InterfaceElementIndex index);
	//void AddCollider(AbstractCollider* collider) { RenderList.push_back(entity); }

public:

	static InterfaceManager* GetInstance()
	{
		if (interfaceManager == nullptr)
		{
			interfaceManager = new InterfaceManager();
			interfaceManager->Init();
		}
		return interfaceManager;
	};

	InterfaceElement* GetElementAt(vec2 pos)
	{
		std::vector<InterfaceElement*> *ElementsToCheck = &RenderList;
		return ElementAt(pos, ElementsToCheck);
	}

	InterfaceElement* ElementAt(vec2 pos, std::vector<InterfaceElement*>* ElementsToCheck)
	{
		for (int i = 0; i < ElementsToCheck->size(); i++)
		{
			InterfaceElement* IElement = ElementsToCheck->at(i);

			vec2 IEPosition = IElement->GetPosition();
			float IEHeight = IElement->GetHeight();
			float IEWidth = IElement->GetWidth();

			std::vector<InterfaceElement*>* ChildrenElement = IElement->GetChildren();
			InterfaceElement* SucessorAt = ElementAt(pos, ChildrenElement);
			if (SucessorAt != NULL)
				return SucessorAt;

			if (IEPosition.x <= pos.x && IEPosition.x + IEWidth >= pos.x)
			{
				if (IEPosition.y >= pos.y && pos.y >= IEPosition.y - IEHeight)
				{
					return IElement;
				}
			}
		}

		return NULL;
	}
};


