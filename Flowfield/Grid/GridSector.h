#ifndef GRIDSECTOR 
#define GRIDSECTOR

#include "../../Utils.h"
#include "../Portal/Portal.h"
#include "Grid.h"
#include <functional>

#include <vector>
#include <memory>
#include <list>

class GridSector final
{
public:
    //Lookup table for the directions
    enum class Directions
    {
        N = 0,
        NE = 1,
        E = 2,
        SE = 3,
        S = 4,
        SW = 5,
        W = 6,
        NW = 7,

        INVALID = 8
    };

	GridSector(const utils::Point2i& bottomLeft);
	~GridSector() = default;

    GridSector(const GridSector&) = delete;
    GridSector& operator=(const GridSector&) = delete;
    GridSector(GridSector&&) noexcept = delete;
    GridSector& operator=(GridSector&&) noexcept = delete;
    
    void Draw() const;

    Directions GetFlowFieldOutput(const utils::Point2i& agentPos);

    bool CalculateFlowField(const Portal& portal, const Grid::GetIntegrationCostFromCellIdx& costFunction);

    uint16_t GetIntegrationFromIdx(unsigned idx) const;

	[[nodiscard]] utils::Recti GetBounds() const noexcept;

    bool FreeSector() noexcept;
    bool LoadSector() noexcept;

    bool IsActive() const;

private:
    const utils::Point2i m_BottomLeft;
    static const unsigned m_CellSize{50};

    static const unsigned m_Rows {10};
    static const unsigned m_Cols {10};

    bool m_IsActive{ false };

    //Debugging
    std::vector<unsigned> m_GoalList;
    
    //The cost field used for the grid, each value ranges from 0 - 255; all minimum cost of 1 and 255 is a wall
    std::vector<uint8_t> m_CostField;
    static const std::vector<uint8_t> m_ClearCostField;

    //8 bit int is for flags
    //std::vector<std::pair<uint16_t, uint8_t>> m_IntegrationField;
    std::vector<uint16_t> m_IntegrationField;

    //Direction and flags for the grid
    std::vector<uint8_t> m_FlowField;
private:
    bool InitCostField();
    bool CalculateIntegrationField(const Portal& portal, const Grid::GetIntegrationCostFromCellIdx& costFunction) noexcept;

    void DrawGrid() const;
    void DrawCostField() const;
    void DrawFlowField() const;
    void DrawIntegrationField() const;
    void DrawPortals() const;

    std::vector<unsigned> GetIntegrationNeighbors(unsigned col, unsigned row) const;
    uint16_t GetFlowFieldNeighborCostForPortal(Directions direction, unsigned row, unsigned col, const Portal& portal, const Grid::GetIntegrationCostFromCellIdx& costFunction) const;

    bool IsValidIntegrationNeighborIdx(Directions direction, unsigned row, unsigned col, const Portal& portal) const;

    unsigned PositionToIdx(unsigned x, unsigned y) const;
};
#endif