//-----------------------------------------------------------------
// Main Game  File
// C++ Header - Flowfields.h - version v7_02			
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------

#include "Resource.h"	
#include "GameEngine.h"
#include "AbstractGame.h"
#include "Utils.h"
#include <memory>
#include <vector>

class Grid;
class Agent;

//-----------------------------------------------------------------
// Flowfields Class																
//-----------------------------------------------------------------
class Flowfields : public AbstractGame, public Callable
{
public:				
	//---------------------------
	// Constructor(s) and Destructor
	//---------------------------
	Flowfields();

	virtual ~Flowfields() override;

	//---------------------------
	// Disabling copy/move constructors and assignment operators   
	//---------------------------
	Flowfields(const Flowfields& other) = delete;
	Flowfields(Flowfields&& other) noexcept = delete;
	Flowfields& operator=(const Flowfields& other) = delete;
	Flowfields& operator=(Flowfields&& other) noexcept = delete;

	//---------------------------
	// General Methods
	//---------------------------
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
	// -------------------------
	// Datamembers
	// -------------------------
	std::unique_ptr<Grid> m_Grid;
	std::unique_ptr<Font> m_pFont;

	std::vector<std::unique_ptr<Agent>> m_pAgents;

	void InitRandomAgents();
};
