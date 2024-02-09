#include "MapEditor.h"

MapEditor::MapEditor()
{
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
}

void MapEditor::DrawLoadingMap() const
{

}

void MapEditor::DrawCreatingMap() const
{

}
