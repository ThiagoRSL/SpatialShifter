
#include "InterfaceManager.h"
#include "InputManager.h"


using namespace std;

InterfaceManager* InterfaceManager::interfaceManager = nullptr;


InterfaceManager::InterfaceManager()
{
	MouseHoverElement = NULL;
	vec2 windowSize = InputManager::GetInstance()->GetWindowSize();
	RootInterfaceElement = new InterfaceElement();
	RootInterfaceElement->SetWidth(windowSize.x);
	RootInterfaceElement->SetHeight(windowSize.y);

	RenderList = std::vector<InterfaceElement*>();
	DictIndexedInterfaceElements = std::map<InterfaceElementIndex, InterfaceElement*>();
	InterfaceShader = new Shader();
}

void InterfaceManager::RenderInterface()
{
	for (int i = 0; i < RenderList.size(); i++)
	{
		//Inicia a chamada recursiva de renderização.
		RenderList.at(i)->Render();
	}
}


void InterfaceManager::AddInterfaceComponent(InterfaceElement* elem, InterfaceElementIndex index)
{
	this->RootInterfaceElement->AddInterfaceComponent(elem);
	DictIndexedInterfaceElements[index] = elem;
	elem->SetElementIndex(index);
}

void InterfaceManager::ToggleRender(InterfaceElementIndex index)
{
	InterfaceElement* ie = DictIndexedInterfaceElements[index];
	if (ie == NULL)
		return;

	for (int i = 0; i < RenderList.size(); i++)
	{
		if (RenderList.at(i) == ie)
		{
			UnsetToRender(index);
			return;
		}
	}
	SetToRender(index);
}

InterfaceElement* InterfaceManager::GetIndexedElement(InterfaceElementIndex index)
{
	if (DictIndexedInterfaceElements[index] != NULL)
		return DictIndexedInterfaceElements[index];
	return nullptr;
}

void InterfaceManager::SetToRender(InterfaceElementIndex index)
{
	if (DictIndexedInterfaceElements[index] != NULL)
		RenderList.push_back(DictIndexedInterfaceElements[index]);
}

void InterfaceManager::UnsetToRender(InterfaceElementIndex index)
{
	InterfaceElement* ie = DictIndexedInterfaceElements[index];
	if (ie == NULL)
		return;

	auto it = RenderList.begin();
	while (it != RenderList.end())
	{
		if (*it == ie)
		{
			it = RenderList.erase(it);
			return;
		}
		else
		{
			++it;
		}
	}
	return;
}


void InterfaceManager::RenderElement(InterfaceElementIndex index)
{
	DictIndexedInterfaceElements[index]->Render();
}


/*
	Elementos de Interface
*/

InterfaceElement::InterfaceElement()
{
	this->DebugHighlight = false;
	this->HasTexture = false;
	this->IEShader = ShaderManager::GetInstance()->GetInterfaceElementShader();
	this->FatherElement = nullptr;
	this->ElementIndex = InterfaceElementIndex::INDEX_LESS;
	this->Placement = ElementPlacement::ABSOLUTE_PLACEMENT;
	this->RelativePosition = vec2(0.0f, 0.0f);
	this->height = 0.0f;
	this->width = 0.0f;
	this->backgroundColor = vec4(0.0f);	//Background Transparente
	//vec4 borderColor;


	glGenVertexArrays(1, &InterfaceElementVBOID);	//Aloca memória na placa de Vídeo.
	glBindVertexArray(InterfaceElementVBOID);		//Define para usar esse espaço alocado
	this->VboID = new GLuint[3];
	glGenBuffers(3, VboID);
	glBindVertexArray(0);

	VertexIndexes = std::vector<int>();
	Vertexes = std::vector<vec2>();
	TexCoord = std::vector<vec2>();
	Indexes = std::vector<int>();

	this->ChildrenElements = std::vector<InterfaceElement*>();
	this->TextComponents = std::vector<TextElement*>();
}

void InterfaceElement::AddInterfaceComponent(InterfaceElement* elem)
{
	this->ChildrenElements.push_back(elem);
	elem->SetFather(this);
}

void InterfaceElement::Render()
{
	if (HasTexture)
	{
		IEShader = ShaderManager::GetInstance()->GetInterfaceElementShader();
		if (IEShader == NULL)
		{
			IEShader = new Shader();
			printf("\n COMPILANDO SHADERS (IE).");
			IEShader->Compile("shader/interface/interfaceElement.vert", "shader/interface/interfaceElement.frag");
			ShaderManager::GetInstance()->SetInterfaceElementShader(IEShader);
			// TODO: REDEFINIR QUANDO RESIZE
			IEShader->setUniform(string("InterfaceProjection"), InterfaceManager::GetInstance()->GetInterfaceProjection());
		}
		IEShader->Use();
		// TODO: MELHOR USO DAS UNIDADES DE TEXTURA
		IEShader->setUniform(string("BackgroundTexture"), 0);
		IEShader->setUniform(string("BackgroundColor"), this->backgroundColor);


		IEShader->setUniform(string("Position"), this->GetPosition());
		//Renderiza o Elemento
		// TODO: MELHOR USO DAS UNIDADES DE TEXTURA



		glActiveTexture(GL_TEXTURE0);
		TextureManager::Inst()->BindTexture(TextureIndex);
		glBindVertexArray(InterfaceElementVBOID);
		glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (GLubyte*)NULL);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	for (int i = 0; i < this->TextComponents.size(); i++)
	{
		TextManager::GetInstance()->RenderText(this->TextComponents.at(i));
	}

	this->RenderChildren();
}

void InterfaceElement::RenderChildren()
{
	for (int i = 0; i < this->ChildrenElements.size(); i++)
	{
		this->ChildrenElements.at(i)->Render();
	}
}

std::vector<InterfaceElement*>* InterfaceElement::GetChildren()
{
	std::vector<InterfaceElement*>* elements = new std::vector<InterfaceElement*>();

	for (int i = 0; i < ChildrenElements.size(); i++)
	{
		elements->push_back(ChildrenElements.at(i));
	}

	return elements;
}


vec2 InterfaceElement::GetPosition()
{
	//Se o elemento for absoluto, retorna a posição.
	if (Placement == ElementPlacement::ABSOLUTE_PLACEMENT)
	{
		return this->RelativePosition;
	}
	//	Se o posicionamento do elemento for relativo, retorna a posição relativa.
	//	Se o objeto tiver um pai, pega a posição do pai e soma à dele.
	if (Placement == ElementPlacement::RELATIVE_PLACEMENT)
	{
		if (this->FatherElement == NULL)
			return this->RelativePosition;

		return this->FatherElement->GetPosition() + this->RelativePosition;
	}
	return vec2(0.0f, 0.0f);
}

void InterfaceElement::AddText(std::string Text)
{
	TextElement* telem = new TextElement();
	telem->Position = this->GetPosition();
	telem->Text = Text;
	telem->TextColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	telem->Scale = 1.0f;
	telem->LineSpacing = 1.0f;
	telem->LineWidthMax = this->GetWidth();
	telem->MarginLeft = 0.0f;
	telem->MarginTop = 0.0f;

	this->TextComponents.push_back(telem);
}

//Criado pois quando um elemento sofre alteração (troca de pai por exemplo)
//As componentes dele podem precisar ser recalculadas.
void InterfaceElement::UpdateComponents()
{
	for (int i = 0; i < this->TextComponents.size(); i++)
	{
		this->TextComponents.at(i)->Position = this->GetPosition();
	}

	//Chama a atualização nos filhos,
	//NÃO PODE TER LOOP NA ARVORE FAMILIAR DOS ELEMENTOS,
	//TODO: GARANTIR QUE NÃO TENHA LOOP, ISSO PODE GERAR LOOPS INFINITOS DE ATUALIZAÇÕES.
	//PARA EVITAR COMPLICAÇÕES, TALVEZ DEFINIR O PAI NA CRIAÇÃO DO INTERFACE ELEMENT (E GARANTIR QUE NÃO SEJA ALTERAVEL)
	//SEJA O MELHOR A SE FAZER
	for (int i = 0; i < this->ChildrenElements.size(); i++)
	{
		this->ChildrenElements.at(i)->UpdateComponents();
	}

	this->RenderChildren();
}

void InterfaceManager::Init()
{
	//this->InterfaceShader = ShaderManager::GetInstance()->GetInterfaceElementShader();
}

void InterfaceElement::SetElementIndex(InterfaceElementIndex index)
{
	this->ElementIndex = index;
	InterfaceManager::GetInstance()->SetElementIndexed(this, index); 
}

void InterfaceElement::Resize()
{
	GenerateBuffers();
}

void InterfaceElement::GenerateBuffers()
{
	VertexIndexes.clear();
	Vertexes.clear();
	TexCoord.clear();
	Indexes.clear();

	VertexIndexes.push_back(0);
	VertexIndexes.push_back(1);
	VertexIndexes.push_back(2);
	VertexIndexes.push_back(3);

	Vertexes.push_back(vec2(0, 0));
	Vertexes.push_back(vec2(this->width, 0));
	Vertexes.push_back(vec2(this->width, -this->height));
	Vertexes.push_back(vec2(0, -this->height));

	TexCoord.push_back(vec2(0.0, 0.0));
	TexCoord.push_back(vec2(1.0, 0.0));
	TexCoord.push_back(vec2(1.0, 1.0));
	TexCoord.push_back(vec2(0.0, 1.0));

	Indexes.push_back(0);
	Indexes.push_back(1);
	Indexes.push_back(3);
	Indexes.push_back(2);

	glBindVertexArray(InterfaceElementVBOID);		//Define para usar esse espaço alocado

	//Habilita transparência
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glBindBuffer(GL_ARRAY_BUFFER, VboID[0]);
	glBufferData(GL_ARRAY_BUFFER, Vertexes.size() * sizeof(vec2), (GLvoid*)&Vertexes[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
	glEnableVertexAttribArray(0);  // VertexPosition -> layout 0 in the VS

	glBindBuffer(GL_ARRAY_BUFFER, VboID[1]);
	glBufferData(GL_ARRAY_BUFFER, TexCoord.size() * sizeof(vec2), (GLvoid*)&TexCoord[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
	glEnableVertexAttribArray(1);  // TexCoord -> layout 2 in the VS

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboID[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indexes.size() * sizeof(int), (GLvoid*)&Indexes[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void InterfaceElement::SetTexture(std::string NewTexture)
{

	//if (HasTexture)
	//	TextureManager::Inst()->UnloadTexture(BackgroundTextureID);
	if (!HasTexture) //else
	{
		GenerateBuffers();
	}

	TextureIndex = -1;
	TextureIndex = TextureManager::Inst()->GetTexture(NewTexture.c_str());
	
	if (TextureIndex != -1)
	{
		HasTexture = true;
	}

}


void InterfaceElement::Interact(InterfaceInteraction interaction)
{
	switch (interaction)
	{
		case InterfaceInteraction::MOUSE_CLICK_RIGHT:
			printf("MOUSE CLICK (RIGHT) INTERFACE ELEMENT INTERACTION");
		break;
		case InterfaceInteraction::MOUSE_CLICK_LEFT:
			printf("MOUSE CLICK (LEFT) INTERFACE ELEMENT INTERACTION");
			break;
		case InterfaceInteraction::MOUSE_HOVER:
			printf("MOUSE HOVER INTERFACE ELEMENT INTERACTION");
			//SetTexture(GlobalPaths::PARTICLE_ENGINE_PATH);
			break;
		case InterfaceInteraction::MOUSE_LEAVE:
			printf("MOUSE LEAVE INTERFACE ELEMENT INTERACTION");
			break;
		default:
			break;
	}
}