#include "UI.h"

#include "../Engine/GameEngine.h"
#include "../Utils.h"

#include "../MapEditor/MapEditor.h"

UI::UI(Callable* pTarget):
	m_pTarget{pTarget}
{
	InitMainScreen();
}

void UI::RemoveUI()
{
	m_pCurrScreen = std::make_unique<UIScreen>(m_pTarget);

	auto switchToMainScr = [this]()
		{
			this->SwitchToMainScreen();
		};

	auto pHideBtn{ new Button{_T("X")} };
	pHideBtn->SetClickCallback(switchToMainScr);

	auto pEditorElement{ new UIElement{ m_pTarget, pHideBtn, utils::Recti{GAME_ENGINE->GetWidth() - 25, 0, 25, 50}}};

	m_pCurrScreen->AddElement(pEditorElement);
}

void UI::InitMainScreen()
{
	m_pCurrScreen = std::make_unique<UIScreen>(m_pTarget);

	auto switchToMapEditorScr = [this]() 
	{
		this->SwitchToMapEditorScreen();
	};

	auto pEdtiorBtn{ new Button{_T("Go To Map Editor")}};
	pEdtiorBtn->SetClickCallback(switchToMapEditorScr);

	auto pEditorElement{ new UIElement{ m_pTarget, pEdtiorBtn, utils::Recti{0, GAME_ENGINE->GetHeight() - 50, 200, 50}}};

	auto hideUI = [this]()
		{
			this->RemoveUI();
		};

	auto pHideBtn{ new Button{_T("X")} };
	pHideBtn->SetClickCallback(hideUI);

	auto pEditorElement2{ new UIElement{ m_pTarget, pHideBtn, utils::Recti{GAME_ENGINE->GetWidth() - 25, 0, 25, 50}} };

	m_pCurrScreen->AddElement(pEditorElement);
	m_pCurrScreen->AddElement(pEditorElement2);
}

void UI::InitMapEditorScreen()
{
	m_pCurrScreen = std::make_unique<UIScreen>(m_pTarget);

	auto switchToMainScr = [this]()
		{
			this->SwitchToMainScreen();
		};

	auto pMainScrBtn{ new Button{_T("Go To Main Menu")} };
	pMainScrBtn->SetClickCallback(switchToMainScr);
	
	auto pEditorElement{ new UIElement{ m_pTarget, pMainScrBtn, utils::Recti{0, GAME_ENGINE->GetHeight() - 50, 200, 50}} };

	auto hideUI = [this]()
		{
			this->RemoveUI();
		};

	auto pHideBtn{ new Button{_T("X")} };
	pHideBtn->SetClickCallback(hideUI);

	auto pEditorElement2{ new UIElement{ m_pTarget, pHideBtn, utils::Recti{GAME_ENGINE->GetWidth() - 25, 0, 25, 50}} };

	auto loadMap = []()
		{
			MapEditor::LoadMap("MapEditor/Maps/TestMap.txt");
		};

	auto pLoadMapBtn{ new Button{_T("Load Map")} };
	pLoadMapBtn->SetClickCallback(loadMap);

	auto pEditorElement3{ new UIElement{ m_pTarget, pLoadMapBtn, utils::Recti{0 + 400, GAME_ENGINE->GetHeight() - 50, 200, 50}} };

	m_pCurrScreen->AddElement(pEditorElement);
	m_pCurrScreen->AddElement(pEditorElement2);
	m_pCurrScreen->AddElement(pEditorElement3);
}

void UI::SwitchToMainScreen()
{
	InitMainScreen();
}

void UI::SwitchToMapEditorScreen()
{
	InitMapEditorScreen();
}
