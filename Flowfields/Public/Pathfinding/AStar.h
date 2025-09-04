#ifndef ASTAR 
#define ASTAR

class Graph;
class GraphNode;

#include <vector>
#include <queue>

#include <unordered_map>
#include <limits>

class AStar final
{
public:
	AStar(Graph* pGraph);

	std::vector<GraphNode*> FindPath(GraphNode* pStartNode, GraphNode* pDestinationNode);

private:
    struct AStarNode
    {
        GraphNode* node;
        float gCost; 
        float hCost; 

        bool operator<(const AStarNode& other) const
        {
            return gCost + hCost > other.gCost + other.hCost;
        }
    };

    std::priority_queue<AStarNode> m_OpenSet;

    std::unordered_map<GraphNode*, float> m_gCosts;  // cost from start to each node
    std::unordered_map<GraphNode*, GraphNode*> m_CameFrom;  // parent node for each node

	Graph* m_pGraph;

    float CalculateHeuristic(GraphNode* pStartNode, GraphNode* pDestNode) const;
    std::vector<GraphNode*> ReconstructPath(GraphNode* pStart, GraphNode* pCurr) const;
};

#endif

