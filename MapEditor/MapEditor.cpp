#include "MapEditor.h"

MapEditor::MapEditor(Callable* callablePtr)
{
	m_pLoadMapBtn = std::make_unique<Button>();
	m_pLoadMapBtn->SetBounds(GAME_ENGINE->GetWidth() - 100, GAME_ENGINE->GetHeight() - 25, 100, 25);
	m_pLoadMapBtn->AddActionListener(callablePtr);
	m_pLoadMapBtn->Show();

}

void MapEditor::Draw() const
{
	if (m_Display)
	{	
		switch (m_EditorState)
		{
		case MapEditor::EditorState::LoadingMap:
			DrawLoadingMap();
			break;

		case MapEditor::EditorState::CreatingMap:
			DrawCreatingMap();
			break;
		}
	}
}

void MapEditor::ToggleDisplay()
{
	m_Display = !m_Display;
}

void MapEditor::CallAction(Caller* callerPtr)
{
	if (callerPtr == m_pLoadMapBtn.get())
	{
		//Tetxbox to type map name to load
	}
}

void MapEditor::DrawLoadingMap() const
{

}

void MapEditor::DrawCreatingMap() const
{

}
