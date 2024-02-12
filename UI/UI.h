#ifndef USERINTERFACE
#define USERINTERFACE

#include "UIScreen.h"
#include <memory>

class UI final
{
public:
	UI(Callable* pTarget);
	~UI() = default;

	UI(const UI& other) = delete;
	UI(UI&& other) noexcept = delete;
	UI& operator=(const UI& other) = delete;
	UI& operator=(UI&& other) noexcept = delete;

	void RemoveUI();

private:
	std::unique_ptr<UIScreen> m_pCurrScreen;

	Callable* m_pTarget;

	void InitMainScreen();
	void InitMapEditorScreen();

	void SwitchToMainScreen();
	void SwitchToMapEditorScreen();
};

#endif

