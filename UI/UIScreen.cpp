#include "UIScreen.h"

UIScreen::UIScreen(Callable* pTarget)
{
	m_pUIElements.push_back(std::make_unique<UIElement>(pTarget, std::make_unique<Button>()));
}

void UIScreen::Draw() const
{

}
