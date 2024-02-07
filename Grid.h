#ifndef GRID 
#define GRID

#include "Utils.h"
#include "Graph.h"
#include "Portal.h"

#include <vector>
#include <memory>
#include <functional>

class GridSector;

class Grid final
{
public:
    Grid();
    ~Grid() = default;

    Grid(const Grid&) = delete;
    Grid& operator=(const Grid&) = delete;
    Grid(Grid&&) noexcept = delete;
    Grid& operator=(Grid&&) noexcept = delete;

    void Draw() const;

    void UpdateGrid();

    bool UpdateGoalPos(int x, int y);
    bool UpdateSourcePos(int x, int y);

    using GetIntegrationCostFromCellIdx = std::function<uint16_t(unsigned toFieldId, unsigned cellIdx)>;

private:
    bool m_RecalculateGrid{ false };
    Point2i m_SourcePos{20, 200};
    Point2i m_GoalPos;

    Grid::GetIntegrationCostFromCellIdx m_CostFunction;

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
};

#endif

