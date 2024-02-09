#ifndef USERINTERFACE
#define USERINTERFACE

#include "UIScreen.h"
#include <memory>


class UI
{
public:
	UI(Callable* pTarget);
	~UI() = default;

	UI(const UI& other) = delete;
	UI(UI&& other) noexcept = delete;
	UI& operator=(const UI& other) = delete;
	UI& operator=(UI&& other) noexcept = delete;

	void CallAction(Caller* callerPtr);


private:
	std::unique_ptr<UIScreen> m_pMainUIScreen;

	UIScreen* m_pCurrScreen;

	void InitializeScreens(Callable* pTarget);

	void InitMainScreen(Callable* pTarget);
};

#endif

