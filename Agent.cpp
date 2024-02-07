#include "Agent.h"
#include "Engine/GameEngine.h"
#include "Flowfield/Grid/GridSector.h"

Agent::Agent(const Point2i& pos):
	m_Pos{pos}
{
}

void Agent::Render() const
{
	constexpr auto agentColor{ RGB(255, 1, 1) };

	GAME_ENGINE->SetColor(agentColor);
	GAME_ENGINE->FillOval(m_Pos.x, m_Pos.y, m_AgentSize, m_AgentSize);
}

void Agent::Update(GridSector* pGridS)
{
	auto dir{ pGridS->GetFlowFieldOutput(m_Pos) };

	if (dir == GridSector::Directions::INVALID)
	{
		return;
	}
	
	switch (dir)
	{
	case GridSector::Directions::N:
		m_Pos.x;
		m_Pos.y -= 1;
		break;
	case GridSector::Directions::NE:
		m_Pos.x += 1;
		m_Pos.y -= 1;
		break;
	case GridSector::Directions::E:
		m_Pos.x += 1;
		m_Pos.y;
		break;
	case GridSector::Directions::SE:
		m_Pos.x += 1;
		m_Pos.y += 1;
		break;
	case GridSector::Directions::S:
		m_Pos.x;
		m_Pos.y += 1;
		break;
	case GridSector::Directions::SW:
		m_Pos.x -= 1;
		m_Pos.y += 1;
		break;
	case GridSector::Directions::W:
		m_Pos.x -= 1;
		m_Pos.y;
		break;
	case GridSector::Directions::NW:
		m_Pos.x -= 1;
		m_Pos.y -= 1;
		break;
	}

}
