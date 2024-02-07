//-----------------------------------------------------------------
// Main Game File
// C++ Source - Flowfields.cpp - version v7_02
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Flowfields.h"																				
#include "Grid/Grid.h"
#include "Grid/GridSector.h"
#include "../Agent.h"

//-----------------------------------------------------------------
// Flowfields methods																				
//-----------------------------------------------------------------

Flowfields::Flowfields() 																	
{
	// nothing to create
}

Flowfields::~Flowfields()																						
{
	// nothing to destroy
}

void Flowfields::Initialize(HINSTANCE hInstance)			
{
	// Set the required values
	AbstractGame::Initialize(hInstance);
	GAME_ENGINE->SetTitle(_T("Flowfields"));					
	GAME_ENGINE->RunGameLoop(true);		
	
	// Set the optional values
	GAME_ENGINE->SetWidth(1024);
	GAME_ENGINE->SetHeight(768);
    GAME_ENGINE->SetFrameRate(50);

	// Set the keys that the game needs to listen to
	//tstringstream buffer;
	//buffer << _T("KLMO");
	//buffer << (TCHAR) VK_LEFT;
	//buffer << (TCHAR) VK_RIGHT;
	//GAME_ENGINE->SetKeyList(buffer.str());

}

void Flowfields::Start()
{
	m_pFont = std::make_unique<Font>(_T("Courier New"), false, false, false, 20);
	m_Grid = std::make_unique<Grid>();

	//m_pAgents.emplace_back(std::make_unique<Agent>());
	//InitRandomAgents();

	GAME_ENGINE->SetFont(m_pFont.get());
}

void Flowfields::End()
{
}

void Flowfields::Paint(RECT rect)
{
	GAME_ENGINE->DrawSolidBackground(RGB(0, 0, 255));
	m_Grid->Draw();

	for (const auto& pAgent : m_pAgents)
	{
		pAgent->Render();
	}
}

void Flowfields::Tick()
{
	m_Grid->UpdateGrid();
}

void Flowfields::MouseButtonAction(bool isLeft, bool isDown, int x, int y, WPARAM wParam)
{	
	if (isLeft && isDown)
	{
		m_Grid->UpdateGoalPos(x, y);
	}
	if (!isLeft && isDown)
	{
		m_Grid->UpdateSourcePos(x, y);
	}
}


void Flowfields::MouseWheelAction(int x, int y, int distance, WPARAM wParam)
{	
	// Insert the code that needs to be executed when the game registers a mouse wheel action
}

void Flowfields::MouseMove(int x, int y, WPARAM wParam)
{	
	// Insert the code that needs to be executed when the mouse pointer moves across the game window

	/* Example:
	if ( x > 261 && x < 261 + 117 ) // check if mouse position is within x coordinates of choice
	{
		if ( y > 182 && y < 182 + 33 ) // check if mouse position also is within y coordinates of choice
		{
			GAME_ENGINE->MessageBox(_T("Da mouse wuz here."));
		}
	}
	*/
}

void Flowfields::CheckKeyboard()
{	
	// Here you can check if a key of choice is held down
	// Is executed once per frame if the Game Loop is running 

	/* Example:
	if (GAME_ENGINE->IsKeyDown(_T('K'))) xIcon -= xSpeed;
	if (GAME_ENGINE->IsKeyDown(_T('L'))) yIcon += xSpeed;
	if (GAME_ENGINE->IsKeyDown(_T('M'))) xIcon += xSpeed;
	if (GAME_ENGINE->IsKeyDown(_T('O'))) yIcon -= ySpeed;
	*/
}

void Flowfields::KeyPressed(TCHAR cKey)
{	
	// DO NOT FORGET to use SetKeyList() !!

	// Insert the code that needs to be executed when a key of choice is pressed
	// Is executed as soon as the key is released
	// You first need to specify the keys that the game engine needs to watch by using the SetKeyList() method

	/* Example:
	switch (cKey)
	{
	case _T('K'): case VK_LEFT:
		GAME_ENGINE->MessageBox(_T("Moving left."));
		break;
	case _T('L'): case VK_DOWN:
		GAME_ENGINE->MessageBox(_T("Moving down."));
		break;
	case _T('M'): case VK_RIGHT:
		GAME_ENGINE->MessageBox(_T("Moving right."));
		break;
	case _T('O'): case VK_UP:
		GAME_ENGINE->MessageBox(_T("Moving up."));
		break;
	case VK_ESCAPE:
		GAME_ENGINE->MessageBox(_T("Escape menu."));
	}
	*/
}

void Flowfields::CallAction(Caller* callerPtr)
{
	// Insert the code that needs to be executed when a Caller has to perform an action
}

void Flowfields::InitRandomAgents()
{
	//constexpr auto AMT_OF_AGENTS{ 10 };

	//auto& bounds{ m_Grid->GetBounds() };

	//for (size_t i{ 0 }; i < AMT_OF_AGENTS; ++i)
	//{
	//	const int x{ rand() % bounds.width + bounds.x + 1};
	//	const int y{ rand() % bounds.height + (GAME_ENGINE->GetHeight() - bounds.y + 1)};

	//	m_pAgents.emplace_back(std::make_unique<Agent>( utils::Point2i{ x, y } ));
	//}
}