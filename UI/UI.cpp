#include "UI.h"

#include "../Engine/GameEngine.h"
#include "../Utils.h"

UI::UI(Callable* pTarget)
{
	InitializeScreens(pTarget);

	m_pCurrScreen = m_pMainUIScreen.get();
}

void UI::CallAction(Caller* callerPtr)
{
	m_pCurrScreen->CallAction(callerPtr);
}

void UI::InitializeScreens(Callable* pTarget)
{
	InitMainScreen(pTarget);
}

void UI::InitMainScreen(Callable* pTarget)
{
	m_pMainUIScreen = std::make_unique<UIScreen>(pTarget);

	// Create the button with a callback to switch to the main screen
	auto switchToMainCallback = [this]() 
	{
		this->SwitchToMainScreen();
	};

	auto pEdtiorBtn{ new Button{_T("Map Editor")}};
	pEdtiorBtn->SetClickCallback(switchToMainCallback);

	auto pEditorElement{ new UIElement{ pTarget, pEdtiorBtn, utils::Recti{0, 200, 100, 25} } };

	m_pMainUIScreen->AddElement(pEditorElement);
}

void UI::SwitchToMainScreen()
{
	OutputDebugString(_T("MAIN SCREEN _------------------------------"));
}
