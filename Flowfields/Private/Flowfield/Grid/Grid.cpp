#include "pch.h"

#include "Defines.h"

using utils::Point2i;

namespace FF
{
	Grid::Grid()
	{
		m_SourcePos = { 0, GAME_ENGINE->GetHeight() - 1 };
		m_GoalPos = { 0, GAME_ENGINE->GetHeight() - 1 };

		m_pGraph = std::make_unique<Graph>();

		InitGridSectors();
		InitPortals();
		SetupPortalGraph();
		
		m_CostFunction = [this](unsigned toFieldId, unsigned cellIdx)
		{
			return m_pGridSectors[toFieldId]->GetIntegrationFromIdx(cellIdx);
		};
	}

	Grid::~Grid()
	{
	}

	void Grid::Draw() const
	{
		for (const auto& gridSec : m_pGridSectors)
		{
			gridSec->Draw();
		}

		if constexpr (FF::drawGraph)
		{
			m_pGraph->DrawGraph();

			//Draw the path
			GAME_ENGINE->SetColor(RGB(0, 0, 0));
			auto idx = 0;
			for (const auto& p : m_pPath)
			{
				GAME_ENGINE->DrawString((to_tstring(idx)), p->GetPosition().x, p->GetPosition().y);
				idx++;
				//GAME_ENGINE->FillOval(p->GetPosition().x, p->GetPosition().y, 30, 30);
			}
		}

		GAME_ENGINE->SetColor(RGB(0, 0, 0));
		GAME_ENGINE->FillOval(m_GoalPos.x,m_GoalPos.y, 20, 20);
		GAME_ENGINE->FillOval(m_SourcePos.x, m_SourcePos.y, 20, 20);

		GAME_ENGINE->SetColor(RGB(0, 255, 0));
		GAME_ENGINE->DrawRect(m_Bounds.x, m_Bounds.y, m_Bounds.width, m_Bounds.height);

	}

	void Grid::UpdateGrid()
	{
		//TODO cleanup / refactor into more functions
		if (m_RecalculateGrid)
		{
			bool onlyGoalField{ m_GoalGridVecIdx == m_SourceVecIdx };

			RequestPath(m_SourcePos.x, m_SourcePos.y, m_GoalPos.x, m_GoalPos.y);

			std::vector<unsigned> activeGridIdxes{};

			//Create a portal for the goalNode of the graph;
			Portal goalPortal{};
			goalPortal.fromFieldId = m_GoalGridVecIdx;
			goalPortal.toFieldId = m_GoalGridVecIdx;
			goalPortal.FromFieldwindowPostions.emplace_back(m_GoalPos);

			m_pGridSectors[m_GoalGridVecIdx]->LoadSector();
			activeGridIdxes.emplace_back(m_GoalGridVecIdx);

			m_pGridSectors[m_GoalGridVecIdx]->CalculateFlowField(goalPortal, m_CostFunction);


			if (onlyGoalField)
			{
				ResetFields(activeGridIdxes);
				return;
			}

			int id{ -1 };
			std::vector<Portal> portals;

			auto it{ std::find_if(cbegin(m_Portals[m_SourceVecIdx]), cend(m_Portals[m_SourceVecIdx]), [this](const Portal& p) { return p.portalId == m_pPath[1]->GetId(); }) };
			assert(it != end(m_Portals[m_SourceVecIdx]));

			const auto& portal{ *it };

			id = portal.toFieldId;

			portals.emplace_back(portal);

			int pathidx{ 1 };
			while (id != m_GoalGridVecIdx)
			{
				++pathidx;
				it = std::find_if(cbegin(m_Portals[portal.toFieldId]), cend(m_Portals[portal.toFieldId]), [pathidx, this](const Portal& p) { return p.portalId == m_pPath[pathidx]->GetId(); });
				portals.emplace_back(*it);

				id = it->toFieldId;
			}

			for (auto portalIt{ rbegin(portals) }; portalIt != rend(portals); ++portalIt)
			{
				m_pGridSectors[(*portalIt).fromFieldId]->LoadSector();
				activeGridIdxes.emplace_back((*portalIt).fromFieldId);

				m_pGridSectors[(*portalIt).fromFieldId]->CalculateFlowField(*portalIt, m_CostFunction);
			}

			m_RecalculateGrid = false;

			ResetFields(activeGridIdxes);
		}
	}

	bool Grid::UpdateGoalPos(int x, int y)
	{
		const auto newGoalIdx{ PositionToGridSectorIdx(x, y) };

		if (newGoalIdx == -1)
		{
			//Invalid goalPos
			return false;
		}

		//if (newGoalIdx != m_GoalGridVecIdx)
		//{
			m_RecalculateGrid = true;
		//}

		m_GoalGridVecIdx = static_cast<unsigned>(newGoalIdx);
		m_GoalPos = { x, y };

		return true;
	}

	bool Grid::UpdateSourcePos(int x, int y)
	{
		const auto newSourceIdx{ PositionToGridSectorIdx(x, y) };

		if (newSourceIdx == -1)
		{
			//Invalid sourcePos
			return false;
		}

		//if (newSourceIdx != m_SourceVecIdx)
		//{
			m_RecalculateGrid = true;
		//}

		m_SourceVecIdx = static_cast<unsigned>(newSourceIdx);
		m_SourcePos = { x, y };

		return true;
	}

	GridSector* Grid::GetGridSector(Point2i const& position) const
	{
		for (auto const& s : m_pGridSectors)
		{
			auto const b = s->GetBounds();
			if (utils::IsPointInRect(position, b))
			{
				if (s->IsActive())
				{
					return s.get();
				}
			}
		}
		return nullptr; // Not found
	}

	void Grid::SetupPortalGraph() noexcept
	{
		//Connect the portals that should be connected in the graph
		//Portal 0
		m_pGraph->AddConnection(0, 1);
		m_pGraph->AddConnection(0, 2);

		//portal 1
		m_pGraph->AddConnection(1, 3);
		m_pGraph->AddConnection(1, 0);

		//portal 2
		m_pGraph->AddConnection(2, 0);
		m_pGraph->AddConnection(2, 3);

		//portal 3
		m_pGraph->AddConnection(3, 1);
		m_pGraph->AddConnection(3, 2);
	}


	void Grid::RequestPath(int sourceX, int sourceY, int goalX, int goalY) noexcept
	{
		//Remove the previous goalNode if valid
		if (m_pGoalGraphNodeIdx != -1)
		{
			m_pGraph->RemoveNode(m_pGoalGraphNodeIdx);
		}

		if (m_SourceNodeIdx != -1)
		{
			m_pGraph->RemoveNode(m_SourceNodeIdx);
		}

		AStar astar{ m_pGraph.get() };

		//Add the goal as a node in the graph
		m_pGoalGraphNodeIdx = m_pGraph->AddNode(Point2i{ goalX, goalY });
		m_SourceNodeIdx = m_pGraph->AddNode(Point2i{ sourceX, sourceY });

		//Create connections with all portals of the gridsector for the goal
		for (const auto& p : m_Portals[m_GoalGridVecIdx])
		{
			m_pGraph->AddConnection(m_pGoalGraphNodeIdx, p.portalId);
		}

		//Create connections with all portals of the gridsector for the source
		for (const auto& p : m_Portals[m_SourceVecIdx])
		{
			m_pGraph->AddConnection(p.portalId, m_SourceNodeIdx);
		}

		m_pPath = astar.FindPath(m_pGraph->GetNode(m_pGoalGraphNodeIdx), m_pGraph->GetNode(m_SourceNodeIdx));
	}


	void Grid::InitGridSectors() noexcept
	{
		constexpr unsigned amtOfGridSectors{ 4 };

		m_pGridSectors.reserve(amtOfGridSectors);

		AddGridSector(std::make_unique<GridSector>(Point2i{ 0,  GAME_ENGINE->GetHeight() }), true);
		AddGridSector(std::make_unique<GridSector>(Point2i{ 250, GAME_ENGINE->GetHeight() }));
		AddGridSector(std::make_unique<GridSector>(Point2i{ 0,   GAME_ENGINE->GetHeight() - 250 }));
		AddGridSector(std::make_unique<GridSector>(Point2i{ 250, GAME_ENGINE->GetHeight() - 250 }));

		m_Portals.resize(amtOfGridSectors);
	}

	int Grid::PositionToGridSectorIdx(int x, int y) const
	{
		for (size_t idx{0}; idx < m_pGridSectors.size(); ++idx)
		{
			if (utils::IsPointInRect({x, y}, m_pGridSectors[idx]->GetBounds()))
			{
				return idx;
			}
		}

		return -1;
	}

	void Grid::ResetFields(std::vector<unsigned>& activeGridIdxes) noexcept
	{
		for (size_t idx{ 0 }; idx < m_pGridSectors.size(); ++idx)
		{
			auto it{ std::find(begin(activeGridIdxes), end(activeGridIdxes), idx) };
			if (it == end(activeGridIdxes))
			{
				m_pGridSectors[idx]->FreeSector();
			}
			else
			{
				*it = activeGridIdxes.back();
				activeGridIdxes.pop_back();
			}
		}
	}

	void Grid::AddGridSector(std::unique_ptr<GridSector>&& sector, bool first) noexcept
	{
		assert(sector && "Sector must not be null");

		// Add sector
		m_pGridSectors.emplace_back(std::move(sector));

		auto const& bounds{ m_pGridSectors.back()->GetBounds() };

		if (!first)
		{
			assert(bounds.width > 0 && "Sector width must be positive");
			assert(bounds.height > 0 && "Sector height must be positive");

			//assert(not utils::IsOverlapping(bounds, m_Bounds));

			// Update enclosing bounds properly
			int const newMinX{ min(m_Bounds.x, bounds.x) };
			int const newMaxX{ max(m_Bounds.x + m_Bounds.width, bounds.x + bounds.width) };
			m_Bounds.x = newMinX;
			m_Bounds.width = newMaxX - newMinX;

			int const newBottomY{ max(m_Bounds.y, bounds.y) };
			int const newTopY{ min(m_Bounds.y - m_Bounds.height, bounds.y - bounds.height) };
			m_Bounds.y = newBottomY;
			m_Bounds.height = newBottomY - newTopY;
		}
		else
		{
			m_Bounds = bounds;
		}
	}

	void Grid::InitPortals() noexcept
	{
		//This function should be file data in a game;
		constexpr auto cellSize{ 25 };
		constexpr auto amtOfCells{ 10 };

		//portalside is the direction the algorithm is going towards

		auto windowPosLam = [&](const Point2i& portalPos, Portal::PortalSide pSide) -> std::vector<Point2i> 
			{
				std::vector<Point2i> windosPos{};
				windosPos.reserve(amtOfCells);

				switch (pSide) // could be computed at runtime if we just store windowSide too
				{
				case Portal::PortalSide::LEFT:
					for (size_t i{ 0 }; i < amtOfCells; ++i)
					{
						windosPos.emplace_back(Point2i{ portalPos.x + cellSize/2, static_cast<int>((portalPos.y + amtOfCells * cellSize / 2) - cellSize / 2 - i * cellSize)});
					}
					break;

				case Portal::PortalSide::RIGHT:
					for (size_t i{ 0 }; i < amtOfCells; ++i)
					{
						windosPos.emplace_back(Point2i{ portalPos.x - cellSize/2, static_cast<int>((portalPos.y + amtOfCells * cellSize / 2) - cellSize / 2 - i * cellSize)});
					}
					break;

				case Portal::PortalSide::TOP:
					for (size_t i{ 0 }; i < amtOfCells; ++i)
					{
						windosPos.emplace_back(Point2i{ static_cast<int>((portalPos.x - amtOfCells/2 * cellSize) + i * cellSize), portalPos.y + cellSize / 2 });
					}
					break;

				case Portal::PortalSide::BOTTOM:
					for (size_t i{ 0 }; i < amtOfCells; ++i)
					{
						windosPos.emplace_back(Point2i{ static_cast<int>((portalPos.x - amtOfCells/2 * cellSize) + i * cellSize), portalPos.y - cellSize / 2 });
					}
					break;
				}


				return windosPos;
			};

		auto windowIdxLam = [&](Portal::PortalSide windowSide) -> std::vector<unsigned>
			{
				std::vector<unsigned> windowPos{};
				windowPos.reserve(amtOfCells);

				switch (windowSide) // could be computed at runtime if we just store windowSide too
				{
				case Portal::PortalSide::LEFT:
					for (size_t i{ 0 }; i < amtOfCells; ++i)
					{
						windowPos.emplace_back(i * amtOfCells + (amtOfCells - 1));
					}
					break;

				case Portal::PortalSide::RIGHT:
					for (size_t i{ 0 }; i < amtOfCells; ++i)
					{
						windowPos.emplace_back((i * amtOfCells));
					}
					break;

				case Portal::PortalSide::TOP:
					for (size_t i{ 0 }; i < amtOfCells; ++i)
					{
						windowPos.emplace_back((i));
					}
					break;

				case Portal::PortalSide::BOTTOM:
					for (size_t i{ 0 }; i < amtOfCells; ++i)
					{
						windowPos.emplace_back(((amtOfCells * (amtOfCells - 1)) + i));
					}
					break;
				}

				return windowPos;
			};

		const Point2i portal0Pos{ 500 / 2, GAME_ENGINE->GetHeight() - 250 / 2 },
					  portal1Pos{ 250 / 2, GAME_ENGINE->GetHeight() - 500 / 2 },
					  portal2Pos{ 750 / 2, GAME_ENGINE->GetHeight() - 500 / 2 },
					  portal3Pos{ 500 / 2, GAME_ENGINE->GetHeight() - 750 / 2 };

		//Add portals to graph
		m_pGraph->AddNode(portal0Pos);
		m_pGraph->AddNode(portal1Pos);
		m_pGraph->AddNode(portal2Pos);
		m_pGraph->AddNode(portal3Pos);

		const std::vector<unsigned> temp{};

		std::vector<Portal> portalsSector0{};

		portalsSector0.emplace_back(Portal{ portal0Pos, 0, windowPosLam(portal0Pos, Portal::PortalSide::RIGHT), windowIdxLam(Portal::PortalSide::RIGHT), 1, 0});
		portalsSector0.emplace_back(Portal{ portal1Pos, 1, windowPosLam(portal1Pos, Portal::PortalSide::TOP), windowIdxLam(Portal::PortalSide::TOP),  2, 0 });

		m_Portals[0] = portalsSector0;


		std::vector<Portal> portalsSector1{};
		portalsSector1.emplace_back(Portal{ portal0Pos, 0, windowPosLam(portal0Pos, Portal::PortalSide::LEFT), windowIdxLam(Portal::PortalSide::LEFT), 0, 1 });
		portalsSector1.emplace_back(Portal{ portal2Pos, 2, windowPosLam(portal2Pos, Portal::PortalSide::TOP), windowIdxLam(Portal::PortalSide::TOP), 3, 1 });

		m_Portals[1] = portalsSector1;


		std::vector<Portal> portalsSector2{};

		portalsSector2.emplace_back(Portal{ portal1Pos, 1, windowPosLam(portal1Pos, Portal::PortalSide::BOTTOM), windowIdxLam(Portal::PortalSide::BOTTOM), 0, 2 });
		portalsSector2.emplace_back(Portal{ portal3Pos, 3, windowPosLam(portal3Pos, Portal::PortalSide::RIGHT), windowIdxLam(Portal::PortalSide::RIGHT), 3, 2 });

		m_Portals[2] = portalsSector2;


		std::vector<Portal> portalsSector3{};

		portalsSector3.emplace_back(Portal{ portal2Pos, 2, windowPosLam(portal2Pos, Portal::PortalSide::BOTTOM), windowIdxLam(Portal::PortalSide::BOTTOM), 1, 3 });
		portalsSector3.emplace_back(Portal{ portal3Pos, 3, windowPosLam(portal3Pos, Portal::PortalSide::LEFT), windowIdxLam(Portal::PortalSide::LEFT), 2, 3 });

		m_Portals[3] = portalsSector3;
	}
}