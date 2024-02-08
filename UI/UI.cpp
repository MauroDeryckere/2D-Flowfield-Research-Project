#include "UI.h"

UI::UI(Callable* pTarget)
{
	InitializeScreens(pTarget);
}

void UI::Draw() const
{
	m_pMainUIScreen->Draw();
}

void UI::InitializeScreens(Callable* pTarget)
{
	m_pMainUIScreen = std::make_unique<UIScreen>(pTarget);

}
