#ifndef UISCREEN
#define UISCREEN

#include "UIElement.h"
#include <vector>

class UIScreen final
{
public:
	UIScreen(Callable* pTarget);
	~UIScreen() = default;

	UIScreen(const UIScreen& other) = delete;
	UIScreen(UIScreen&& other) noexcept = delete;
	UIScreen& operator=(const UIScreen& other) = delete;
	UIScreen& operator=(UIScreen&& other) noexcept = delete;

	void AddElement(UIElement* pElement);

private:
	std::vector<std::unique_ptr<UIElement>> m_pUIElements;
};

#endif

