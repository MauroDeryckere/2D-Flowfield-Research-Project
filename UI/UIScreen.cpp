#include "UIScreen.h"

UIScreen::UIScreen(Callable* pTarget)
{
}

void UIScreen::AddElement(UIElement* pElement)
{
	m_pUIElements.emplace_back(pElement);
}

void UIScreen::CallAction(Caller* callerPtr)
{

}
