#include "GridSector.h"

#include "../../Engine/GameEngine.h" 
#include "../../DebugSettings.h" 

#include <cassert>
#include <cmath>
#include <list>

//Define the clear costField
const std::vector<uint8_t> GridSector::m_ClearCostField = []() {
	const std::vector<uint8_t> costField(m_Rows * m_Cols, 1);
	//Wall testing
	//costField[6] = 255;
	//costField[7] = 255;
	//costField[8] = 255;
	//costField[16] = 255;
	//costField[18] = 255;
	//costField[26] = 255;
	//costField[27] = 255;
	//costField[28] = 255;
	return costField;
	}();

GridSector::GridSector(const utils::Point2i& bottomLeft) :
	m_BottomLeft{ bottomLeft }
{

}

void GridSector::Draw() const
{
	DrawGrid();

	if (m_IsActive)
	{
		if (DebugSettings::drawPortals)
		{
			DrawPortals();
		}

		if (DebugSettings::drawCostField)
		{
			DrawCostField();
		}
		if (DebugSettings::drawIntegrationField)
		{
			DrawIntegrationField();
		}
		if (DebugSettings::drawFlowField)
		{
			DrawFlowField();
		}
	}
}

GridSector::Directions GridSector::GetFlowFieldOutput(const utils::Point2i& agentPos)
{
	auto x{ agentPos.x };
	auto y{ agentPos.y };

	if (x < m_BottomLeft.x || x > m_BottomLeft.x + m_Cols * m_CellSize
		|| y > m_BottomLeft.y || y < m_BottomLeft.y - m_Rows * m_CellSize)
	{
		return Directions::INVALID;
	}

	auto idx{ PositionToIdx(x, y) };

	return static_cast<Directions>(m_FlowField[idx]);
}

utils::Recti GridSector::GetBounds() const noexcept
{
	const utils::Recti bounds{m_BottomLeft.x, m_BottomLeft.y, m_CellSize * m_Cols, m_CellSize * m_Rows};

	return bounds;
}

bool GridSector::FreeSector() noexcept
{
	if (IsActive())
	{
		m_GoalList.clear();
		m_CostField.clear();
		m_IntegrationField.clear();
		m_FlowField.clear();

		m_IsActive = false;
	}

	return true;
}

bool GridSector::LoadSector() noexcept
{
	if (!IsActive())
	{
		m_CostField.resize(m_Rows * m_Cols);

		InitCostField();

		m_IntegrationField.resize(m_Rows * m_Cols);
		m_FlowField.resize(m_Rows * m_Cols);

		m_IsActive = true;
	}

	return true;
}

bool GridSector::IsActive() const
{
	return m_IsActive;
}

bool GridSector::CalculateFlowField(const Portal& portal, const Grid::GetIntegrationCostFromCellIdx& costFunction)
{
	if (!CalculateIntegrationField(portal, costFunction))
	{
		return false;
	}

    //Direction offsets for each Direction {N, NE, E, SE, S, SW, W, NW} 
	//x offset, y offset
    const std::vector<utils::Point2i> directionOffsets 
	{ {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1} };

	const int AMT_OF_DIRECTIONS{ static_cast<int>(directionOffsets.size()) };

	for (int row{ 0 }; row < m_Rows; ++row)
	{
		for (int col{ 0 }; col < m_Cols; ++col)
		{
			const size_t flowFieldIndex{ row * m_Cols + col };

			uint8_t cheapestCost{ UINT8_MAX };
			Directions cheapestDirection{ Directions::N };

			bool isGoalidx{false};

			if (std::find(cbegin(m_GoalList), cend(m_GoalList), flowFieldIndex) != end(m_GoalList))
			{
				isGoalidx = true;
			}

			for (int dir{ 0 }; dir < AMT_OF_DIRECTIONS; ++dir)
			{
				const int neighborX{ col + directionOffsets[dir].x };
				const int neighborY{ row + directionOffsets[dir].y };

				if (!isGoalidx)
				{
					if (neighborX < 0 || neighborX >= m_Cols || neighborY < 0 || neighborY >= m_Rows)
					{
						continue;
					}

					const unsigned neighborIdx{ neighborY * m_Cols + neighborX };

					if (neighborIdx == flowFieldIndex)
					{
						continue;
					}

					assert(neighborIdx < m_IntegrationField.size());

					const uint16_t neighborCost{ m_IntegrationField[neighborIdx] };

					if (neighborCost < cheapestCost)
					{
						cheapestCost = neighborCost;
						cheapestDirection = static_cast<Directions>(dir);
					}
				}
				else
				{
					const uint16_t neighborCost{ GetFlowFieldNeighborCostForPortal(Directions(dir), row, col, portal, costFunction) };

					if (neighborCost < cheapestCost)
					{
						cheapestCost = neighborCost;
						cheapestDirection = static_cast<Directions>(dir);
					}
				}
			}

            m_FlowField[flowFieldIndex] = static_cast<uint8_t>(cheapestDirection);
        }
    }

    return true;
}

bool GridSector::CalculateIntegrationField(const Portal& portal, const Grid::GetIntegrationCostFromCellIdx& costFunction) noexcept
{
	m_IntegrationField.assign(m_IntegrationField.size(), UINT16_MAX);
	m_GoalList.clear();

	std::list<unsigned> openList{};

	if (portal.fromFieldId == portal.toFieldId)
	{
		auto& pos{ portal.FromFieldwindowPostions[0] };
		unsigned index{ PositionToIdx(pos.x, pos.y) };

		m_IntegrationField[index] = 0;

		openList.emplace_back(index);
		m_GoalList.emplace_back(index);
	}
	else
	{
		for (size_t i{ 0 }; i < portal.toFieldWindowCellIdxes.size(); ++i)
		{
			auto& pos{ portal.FromFieldwindowPostions[i] };
			unsigned index{ PositionToIdx(pos.x, pos.y) };

			if (m_CostField[index] != 255) //Do not push a wall on the integration
			{
				const auto integrateValue{ costFunction(portal.toFieldId, portal.toFieldWindowCellIdxes[i]) };

				if (integrateValue != UINT16_MAX)
				{
					m_IntegrationField[index] = integrateValue + 1;
					openList.emplace_back(index);
				}
				m_GoalList.emplace_back(index); //Still act as if portal even when blocked by wall; allows diagonal pathing
			}
		}
	}

	while (!openList.empty())
	{
		const unsigned currId{ openList.front() };
		openList.pop_front();

		const unsigned currX{ currId % m_Cols };
		const unsigned currY{ currId / m_Cols };

		//Get the N, E, S, and W neighbors of the current node
		const std::vector neighbors{ GetIntegrationNeighbors(currX, currY) };
		const auto neighborCount{ neighbors.size() };

		for (size_t idx{ 0 }; idx < neighborCount; ++idx)
		{				
			const int endNodeCost{ m_IntegrationField[currId] + m_CostField[neighbors[idx]] };

			if (endNodeCost < m_IntegrationField[neighbors[idx]] && m_CostField[neighbors[idx]] != 255) //255 == wall | impassable
			{
				if (std::find(openList.begin(), openList.end(), neighbors[idx]) == openList.end())
				{
					openList.emplace_back(neighbors[idx]);
				}

				m_IntegrationField[neighbors[idx]] = endNodeCost;			
			}
		}
	}
	return true;
}

uint16_t GridSector::GetIntegrationFromIdx(unsigned idx) const
{
	return m_IntegrationField[idx];
}

std::vector<unsigned> GridSector::GetIntegrationNeighbors(unsigned col, unsigned row) const
{
	std::vector<unsigned> neighbors{};

	unsigned idx{ row * m_Cols + col };
	auto isValidIndex = [this](int index) -> bool { return (index >= 0 && index < static_cast<int>(m_Rows * m_Cols)); };

	if (isValidIndex(idx - m_Cols)) {
		neighbors.emplace_back(idx - m_Cols);  // North
	}

	//Could wrap around grid otherwise
	if (col != m_Cols - 1 && isValidIndex(idx + 1)) {
		neighbors.emplace_back(idx + 1);  // East
	}

	if (isValidIndex(idx + m_Cols)) {
		neighbors.emplace_back(idx + m_Cols);  // South
	}

	//Could wrap around grid otherwise
	if (col != 0 && isValidIndex(idx - 1)) {
		neighbors.emplace_back(idx - 1);  // West
	}

	return neighbors;
}

uint16_t GridSector::GetFlowFieldNeighborCostForPortal(Directions direction, unsigned row, unsigned col, const Portal& portal, const Grid::GetIntegrationCostFromCellIdx& costFunction) const
{
	bool isValidNeighbor{ IsValidIntegrationNeighborIdx(direction, row, col, portal) };

	if (!isValidNeighbor)
	{
		return UINT16_MAX;
	}

	const std::vector<utils::Point2i> directionOffsets
	{ {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1} };

	const auto& offset{ directionOffsets[static_cast<int>(direction)] };

	const unsigned neighborRow{ (row + offset.y + m_Rows) % m_Rows };
	const unsigned neighborCol{ (col + offset.x + m_Cols) % m_Cols };

	const unsigned fieldIdx{ unsigned(neighborRow * m_Cols + neighborCol) };

	if (std::find(cbegin(portal.toFieldWindowCellIdxes), cend(portal.toFieldWindowCellIdxes), fieldIdx) == end(portal.toFieldWindowCellIdxes))
	{
		const int neighborX{ static_cast<int>(col + offset.x) };
		const int neighborY{ static_cast<int>(row + offset.y) };

		if (neighborX < 0 || neighborX >= m_Cols || neighborY < 0 || neighborY >= m_Rows)
		{
			return UINT16_MAX;
		}

		const unsigned neighborIdx{ neighborY * m_Cols + neighborX };

		assert(neighborIdx < m_IntegrationField.size());

		return m_IntegrationField[neighborIdx];
	}

	const auto cost{ costFunction(portal.toFieldId, fieldIdx) };

	return cost;
}

bool GridSector::IsValidIntegrationNeighborIdx(Directions direction, unsigned row, unsigned col, const Portal& portal) const
{
	typedef Portal::PortalSide PortalSide;

	PortalSide portalSide{};

	if (portal.portalPos.x == m_BottomLeft.x)
	{
		portalSide = PortalSide::LEFT;
	}
	else if (portal.portalPos.x == m_BottomLeft.x + m_CellSize * m_Cols)
	{
		portalSide = PortalSide::RIGHT;
	}
	else if (portal.portalPos.y == m_BottomLeft.y)
	{
		portalSide = PortalSide::BOTTOM;
	}
	else if (portal.portalPos.y == m_BottomLeft.y - m_CellSize * m_Rows)
	{
		portalSide = PortalSide::TOP;
	}

	if (portalSide == PortalSide::LEFT || portalSide == PortalSide::RIGHT)
	{
		if (row == m_Rows - 1)
		{
			if (direction == Directions::N || direction == Directions::NE || direction == Directions::NW)
			{
				return false;
			}
		}
		else if (row == 0)
		{
			if (direction == Directions::S || direction == Directions::SW || direction == Directions::SE)
			{
				return false;
			}
		}
	}

	if (portalSide == PortalSide::TOP || portalSide == PortalSide::BOTTOM)
	{
		if (col == m_Cols - 1)
		{
			if (direction == Directions::E || direction == Directions::NE || direction == Directions::SE)
			{
				return false;
			}
		}
		else if (col == 0)
		{
			if (direction == Directions::W || direction == Directions::SW || direction == Directions::NW)
			{
				return false;
			}
		}
	}

	return true;
}

unsigned GridSector::PositionToIdx(unsigned x, unsigned y) const
{
	x -= m_BottomLeft.x;
	x /= m_CellSize;

	y = m_BottomLeft.y - y;
	y /= m_CellSize;

	const unsigned index{ y * m_Cols + x};

	assert(index < m_Rows * m_Cols);

	return index;
}

void GridSector::DrawGrid() const
{
	constexpr auto gridColor{ RGB(255,255,255) };
	constexpr auto rasterColor{ RGB(69, 69, 69) };

	GAME_ENGINE->SetColor(gridColor);
	GAME_ENGINE->FillRect(m_BottomLeft.x, m_BottomLeft.y - m_CellSize * m_Rows, m_CellSize * m_Cols, m_CellSize * m_Rows);

	GAME_ENGINE->SetColor(rasterColor);

	for (size_t row{ 0 }; row < m_Rows; ++row)
	{
		for (size_t col{ 0 }; col < m_Cols; ++col)
		{
			if (m_IsActive)
			{
				const size_t fieldIndex{ row * m_Cols + col };

				if (m_IntegrationField[fieldIndex] == UINT16_MAX)
				{
					//Fill rects for walls
					GAME_ENGINE->FillRect(m_BottomLeft.x + m_CellSize * col, m_BottomLeft.y - m_CellSize * (row + 1), m_CellSize, m_CellSize);
				}
			}

			GAME_ENGINE->DrawRect(m_BottomLeft.x + m_CellSize * col, m_BottomLeft.y - m_CellSize * (row + 1), m_CellSize, m_CellSize);
		}
	}

	if (!m_IsActive)
	{
		GAME_ENGINE->SetColor(RGB(255, 1, 1));

		GAME_ENGINE->DrawString(_T("INACTIVE"), m_BottomLeft.x + m_CellSize / 2 * m_Cols, m_BottomLeft.y - m_CellSize/2 * m_Rows);

		return;
	}
}

void GridSector::DrawCostField() const
{
	GAME_ENGINE->SetColor(RGB(255,1,1));

	for (int row{ 0 }; row < m_Rows; ++row)
	{
		for (int col{ 0 }; col < m_Cols; ++col)
		{
			const unsigned fieldIndex{ row * m_Cols + col };

			const int xPos{ static_cast<int>(m_BottomLeft.x + m_CellSize * col) };
			const int yPos{ static_cast<int>(m_BottomLeft.y - m_CellSize * (row + 1)) };

			const auto txt{ std::to_wstring(m_CostField[fieldIndex]) };

			GAME_ENGINE->DrawString((txt), xPos, yPos);
		}
	}
}

void GridSector::DrawFlowField() const
{
	constexpr auto arrColor{ RGB(1, 255, 1)};
	GAME_ENGINE->SetColor(arrColor);

	constexpr int ovalSize{ 10 };

	for (int row{ 0 }; row < m_Rows; ++row)
	{
		for (int col{ 0 }; col < m_Cols; ++col)
		{
			const unsigned idx{ row * m_Cols + col };
			
			const auto dir{ m_FlowField[idx] };

			const int centerX{ static_cast<int>(m_BottomLeft.x + col * m_CellSize + m_CellSize / 2) };
			const int centerY{ static_cast<int>(m_BottomLeft.y - row * m_CellSize - m_CellSize / 2) };


			int endX{ centerX };
			int endY{ centerY };

			const int lineLength{ static_cast<int>( m_CellSize / 2) };

			switch (static_cast<Directions>(dir))
			{
			case GridSector::Directions::N:
				endY -= lineLength;
				break;
			case GridSector::Directions::NE:
				endY -= lineLength;
				endX += lineLength;
				break;
			case GridSector::Directions::E:
				endX += lineLength;
				break;
			case GridSector::Directions::SE:
				endY += lineLength;
				endX += lineLength;
				break;
			case GridSector::Directions::S:
				endY += lineLength;
				break;
			case GridSector::Directions::SW:
				endY += lineLength;
				endX -= lineLength;
				break;
			case GridSector::Directions::W:
				endX -= lineLength;
				break;
			case GridSector::Directions::NW:
				endY -= lineLength;
				endX -= lineLength;
				break;
			default:
				break;
			}

			GAME_ENGINE->DrawOval(centerX - ovalSize/2, centerY, ovalSize, ovalSize);
			GAME_ENGINE->DrawLine(centerX, centerY, endX, endY);
		}
	}
}

void GridSector::DrawIntegrationField() const
{
	GAME_ENGINE->SetColor(RGB(255, 1, 1));

	for (int row{ 0 }; row < m_Rows; ++row)
	{
		for (int col{ 0 }; col < m_Cols; ++col)
		{
			const unsigned fieldIndex{ row * m_Cols + col };

			const int xPos{ static_cast<int>(m_BottomLeft.x + m_CellSize * col) };
			const int yPos{ static_cast<int>(m_BottomLeft.y - m_CellSize * (row + 1)) };

			const auto txt{ std::to_wstring(m_IntegrationField[fieldIndex]) };

			GAME_ENGINE->DrawString((txt), xPos, yPos);
		}
	}
}

void GridSector::DrawPortals() const
{
	constexpr auto goalCellColor{ RGB(1, 1, 255) };

	for (auto goalIdx : m_GoalList)
	{
		const unsigned goalRow{ goalIdx / m_Cols };
		const unsigned goalCol{ goalIdx % m_Rows };

		GAME_ENGINE->SetColor(goalCellColor);
		GAME_ENGINE->FillRect(m_BottomLeft.x + m_CellSize * goalCol, m_BottomLeft.y - m_CellSize * (goalRow + 1), m_CellSize, m_CellSize);
	}
}


bool GridSector::InitCostField()
{
	if (DebugSettings::initRandomCostField)
	{
		for (auto& value : m_CostField)
		{
			value = static_cast<uint8_t>(rand() % 10 + 1);
		}
	}
	else
	{
		m_CostField = m_ClearCostField;
	}

	return true;
}
