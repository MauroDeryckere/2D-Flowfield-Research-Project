#include "UIElement.h"

UIElement::UIElement(Callable* pTarget, std::unique_ptr<Button> pBtn)
{
	pBtn->SetBounds(0, 200, 100, 25);
	pBtn->Show();
	pBtn->SetText(_T("TEST UI EL"));

	m_pElement = std::move(pBtn);

	m_pElement->AddActionListener(pTarget);
}
