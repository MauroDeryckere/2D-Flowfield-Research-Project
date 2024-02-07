#include "AStar.h"

#include "../Graph/Graph.h"
#include "../Graph/GraphNode.h"

AStar::AStar(Graph* pGraph) :
    m_pGraph{ pGraph }
{

}

std::vector<GraphNode*> AStar::FindPath(GraphNode* pStartNode, GraphNode* pDestinationNode)
{
    std::vector<GraphNode*> path{};

    m_OpenSet = {};
    m_gCosts.clear();
    m_CameFrom.clear();

    m_gCosts[pStartNode] = 0.0f;
    const float hCost{ CalculateHeuristic(pStartNode, pDestinationNode) };
    m_OpenSet.push({ pStartNode, 0.0f, hCost });

    while (!m_OpenSet.empty())
    {
        const AStarNode current{ m_OpenSet.top() };
        m_OpenSet.pop();

        GraphNode* const currentNode{ current.node };

        if (currentNode == pDestinationNode)
        {
            path = ReconstructPath(pStartNode, pDestinationNode);
            break;
        }
        
        for (const auto& connection : m_pGraph->GetConnectionsFromNode(currentNode->GetId()) )
        {
            GraphNode* neighborNode = m_pGraph->GetNode(connection->GetToNodeId());

            const float tentativeGCost{ m_gCosts[currentNode] + 1.f }; //Connections do not have a cost, default 1.f

            if (!m_gCosts.count(neighborNode) || tentativeGCost < m_gCosts[neighborNode])
            {
                m_gCosts[neighborNode] = tentativeGCost;
                float hCost = CalculateHeuristic(neighborNode, pDestinationNode);

                m_OpenSet.push({ neighborNode, tentativeGCost, hCost });
                m_CameFrom[neighborNode] = currentNode;
            }
        }
    }

    return path;
}

float AStar::CalculateHeuristic(GraphNode* node, GraphNode* destination) const
{
    return utils::Distance(utils::Vector2f{ static_cast<float>(node->GetPosition().x),         static_cast<float>(node->GetPosition().y) },
                           utils::Vector2f{ static_cast<float>(destination->GetPosition().x),  static_cast<float>(destination->GetPosition().y) });
}
std::vector<GraphNode*> AStar::ReconstructPath(GraphNode* start, GraphNode* current) const
{
    std::vector<GraphNode*> path{};
    while (current != start)
    {
        path.push_back(current);

        auto it{ m_CameFrom.find(current) };

        if (it == m_CameFrom.end())
        {
            //unreachable
            break;
        }

        current = it->second;
    }
    path.push_back(start);
    return path;
}
