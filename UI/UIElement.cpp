#include "UIElement.h"

UIElement::UIElement(Callable* pTarget, Button* pBtn, const utils::Recti& bounds)
{
	pBtn->SetBounds(bounds.x, bounds.y, bounds.width, bounds.height);
	pBtn->Show();

	m_pElement.reset(pBtn);
	m_pElement->AddActionListener(pTarget);
}
