
#include "InterfaceBuilder.h"
using namespace std;


void InterfaceBuilder::BuildInventory()
{
	InterfaceManager* IM = InterfaceManager::GetInstance();

	InterfaceElement* InventoryBody = new InterfaceElement();
	InventoryBody->SetPosition(vec2(500.0f, 500.0f));
	InventoryBody->SetHeight(300.0f);
	InventoryBody->SetWidth(300.0f);
	InventoryBody->AddText(std::string("Inventario lllllllllllllllllllllllllllllllllllllllllllllllllllllllllllll"));

	//ordem que ele é adicionado impacta

	InterfaceElement* InventoryBodySection = new InterfaceElement();
	InventoryBodySection->SetPlacement(ElementPlacement::RELATIVE_PLACEMENT);
	InventoryBodySection->SetPosition(vec2(100.0f, 300.0f));
	InventoryBodySection->SetHeight(300.0f);
	InventoryBodySection->SetWidth(300.0f);
	InventoryBodySection->AddText(std::string("Subsecao do Inventario."));

	InventoryBody->AddInterfaceComponent(InventoryBodySection);
	IM->AddInterfaceComponent(InventoryBody, InterfaceElementIndex::INVENTORY);
}

void InterfaceBuilder::BuildQuickBar()
{
	InterfaceManager* IM = InterfaceManager::GetInstance();
	InterfaceElement* QuickBar = new InterfaceElement();

	float quickSlotSize, quickSlotSpacing, width;
	if (GlobalVars::FULL_SCREEN)
	{
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		width = (float) mode->width;
	}
	else
	{
		width = 1000.0;
	}
	quickSlotSize = 64.0;
	quickSlotSpacing = 20.0;
	std::vector<InterfaceElementIndex> quickSlotIndexes = std::vector<InterfaceElementIndex>();
	quickSlotIndexes.push_back(InterfaceElementIndex::QS1);
	quickSlotIndexes.push_back(InterfaceElementIndex::QS2);
	quickSlotIndexes.push_back(InterfaceElementIndex::QS3);
	quickSlotIndexes.push_back(InterfaceElementIndex::QS4);
	quickSlotIndexes.push_back(InterfaceElementIndex::QS5);
	quickSlotIndexes.push_back(InterfaceElementIndex::QS6);
	quickSlotIndexes.push_back(InterfaceElementIndex::QS7);
	quickSlotIndexes.push_back(InterfaceElementIndex::QS8);
	quickSlotIndexes.push_back(InterfaceElementIndex::QS9);
	quickSlotIndexes.push_back(InterfaceElementIndex::QS0);

	//TODO: REMOVER ISSO DAQUI, PARAMETRIZAR ESSAS CONFIG.
	float totalBarSize = quickSlotSize * quickSlotIndexes.size() + quickSlotSpacing * (quickSlotIndexes.size() - 1); //n slots com n-1 espaçamentos entre eles.
	float barStart = (width - totalBarSize)/2;

	for (int i = 0; i < quickSlotIndexes.size(); i++)
	{
		printf("\n\nVAS\n\n");
		InterfaceElement* qs = new InterfaceElement();

		qs->SetPosition(vec2(barStart + ((quickSlotSize + quickSlotSpacing) * i), 114.0));

		qs->SetHeight(64.0f);
		qs->SetWidth(64.0f);
		qs->SetElementIndex(quickSlotIndexes.at(i));
		qs->SetTexture(GlobalPaths::HOTBAR_EMPTY_SLOT_IMAGE_PATH.c_str());
		
		QuickBar->AddInterfaceComponent(qs);
	}



	//ordem que ele é adicionado impacta
	IM->AddInterfaceComponent(QuickBar, InterfaceElementIndex::QUICK_SLOT_BAR);
	IM->SetToRender(InterfaceElementIndex::QUICK_SLOT_BAR);
}