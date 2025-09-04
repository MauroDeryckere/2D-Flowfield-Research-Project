#include "Graph/GraphNode.h"
#include "GameEngine.h"

void GraphNode::Draw() const
{
	constexpr int nodeSize{ 10 };

	GAME_ENGINE->SetColor(RGB(0, 0, 0));
	GAME_ENGINE->FillOval(m_Position.x - nodeSize/2, m_Position.y - nodeSize/2, nodeSize, nodeSize);
}
