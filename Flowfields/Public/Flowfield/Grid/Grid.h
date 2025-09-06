#ifndef FF_GRID_H
#define FF_GRID_H

#include "../../Utils.h"
#include "../../Graph/Graph.h"
#include "../Portal/Portal.h"

#include <vector>
#include <memory>
#include <functional>

namespace FF
{
    class GridSector;

    class Grid final
    {
    public:
        Grid();
        ~Grid();

        Grid(const Grid&) = delete;
        Grid& operator=(const Grid&) = delete;
        Grid(Grid&&) noexcept = delete;
        Grid& operator=(Grid&&) noexcept = delete;

        void Draw() const;

        void UpdateGrid();

        bool UpdateGoalPos(int x, int y);
        bool UpdateSource(std::vector<std::unique_ptr<class Agent>> const& agents);

        [[nodiscard]] utils::Recti const& GetBounds() const noexcept { return m_Bounds; }
        [[nodiscard]] uint16_t GetIntegrationCostFromCellIdx(unsigned toFieldId, unsigned cellIdx);

    	GridSector* GetGridSector(Point2i const& position) const;

    private:
        bool m_RecalculateGrid{ false };
        Point2i m_SourcePos{ 0, 0 };
        Point2i m_GoalPos{ 0, 0 };

        utils::Recti m_Bounds;

        std::unique_ptr<Graph> m_pGraph;

        std::vector<std::unique_ptr<GridSector>> m_pGridSectors;
        std::vector<std::vector<Portal>> m_Portals; //For every gridsector, store a vector of all Portals in that grid

        std::vector<GraphNode*> m_pPath;

        int m_pGoalGraphNodeIdx{ -1 };
        int m_SourceNodeIdx{ -1 };
        unsigned m_GoalGridVecIdx{ 0 };
        unsigned m_SourceVecIdx{ 2 };

        void InitGridSectors() noexcept;
        void InitPortals() noexcept;
        void SetupPortalGraph() noexcept;

        void RequestPath(int sourceX, int sourceY, int goalX, int goalY) noexcept;

        int PositionToGridSectorIdx(int x, int y) const; //-1 is invalid

        void ResetFields(std::vector<unsigned>& activeGridIdxes) noexcept;

        void AddGridSector(std::unique_ptr<GridSector>&& sector, bool first = false) noexcept;
    };
}

#endif
