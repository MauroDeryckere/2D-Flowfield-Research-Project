#ifndef MAPEDITOR 
#define MAPEDITOR

#include "../Engine/GameEngine.h";

class MapEditor
{
public:

	MapEditor(Callable* callablePtr);
	~MapEditor() = default;

	void Draw() const;

	void ToggleDisplay();

	void CallAction(Caller* callerPtr);



private:
	bool m_Display{false};

	std::unique_ptr<Button> m_pLoadMapBtn;

	enum class EditorState
	{
		LoadingMap = 0,
		CreatingMap = 1
	};

	EditorState m_EditorState;

	void DrawLoadingMap() const;
	void DrawCreatingMap() const;
};

#endif
