#ifndef FF_FLOWFIELDS_H
#define FF_FLOWFIELDS_H

#include "Resource.h"	
#include "GameEngine.h"
#include "AbstractGame.h"
#include "../Utils.h"

#include <memory>
#include <vector>

#include "Grid/Grid.h"

namespace FF
{
	class Agent;

	class Flowfields : public AbstractGame, public Callable
	{
	public:				
		Flowfields();
		virtual ~Flowfields() override;

		Flowfields(const Flowfields&) = delete;
		Flowfields(Flowfields&&) noexcept = delete;
		Flowfields& operator=(const Flowfields&) = delete;
		Flowfields& operator=(Flowfields&&) noexcept = delete;

		void Initialize(HINSTANCE hInstance) override;
		void Start() override;
		void End() override;
		void Paint(RECT rect) override;
		void Tick() override;
		void MouseButtonAction(bool isLeft, bool isDown, int x, int y, WPARAM wParam) override;
		void MouseWheelAction(int x, int y, int distance, WPARAM wParam) override;
		void MouseMove(int x, int y, WPARAM wParam) override;
		void CheckKeyboard() override;
		void KeyPressed(TCHAR cKey) override;
		
		void CallAction(Caller* callerPtr) override;
	private:
		std::unique_ptr<Grid> m_Grid;
		std::unique_ptr<Font> m_pFont;

		std::vector<std::unique_ptr<Agent>> m_pAgents;

		bool m_SetGoal{ false };
		bool m_SetSource{ false };

		void InitRandomAgents();
	};
}

#endif
