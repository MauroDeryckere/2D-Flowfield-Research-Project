#ifndef USERINTERFACE
#define USERINTERFACE

#include "UIScreen.h"

class UI
{
public:
	UI(Callable* pTarget);
	~UI() = default;

	UI(const UI& other) = delete;
	UI(UI&& other) noexcept = delete;
	UI& operator=(const UI& other) = delete;
	UI& operator=(UI&& other) noexcept = delete;

	void Draw() const;

private:
	std::unique_ptr<UIScreen> m_pMainUIScreen;

	void InitializeScreens(Callable* pTarget);
};

#endif

