#ifndef UIELEMENT
#define UIELEMENT

#include <memory>

#include "../Engine/GameEngine.h"

class Caller;

class UIElement
{
public:
	UIElement(Callable* pTarget, std::unique_ptr<Button> pBtn);
	~UIElement() = default;

	UIElement(const UIElement& other) = delete;
	UIElement(UIElement&& other) noexcept = delete;
	UIElement& operator=(const UIElement& other) = delete;
	UIElement& operator=(UIElement&& other) noexcept = delete;



private:
	std::unique_ptr<Caller> m_pElement;

};

#endif

