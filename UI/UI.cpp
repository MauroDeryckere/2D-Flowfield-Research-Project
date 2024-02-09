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

	auto pEdtiorBtn{ new Button{_T("Map Editor")}};
	auto pEditorElement{ new UIElement{ pTarget, pEdtiorBtn, utils::Recti{0, 200, 100, 25} } };

	m_pMainUIScreen->AddElement(pEditorElement);
}
