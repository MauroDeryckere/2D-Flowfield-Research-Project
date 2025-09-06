#include "pch.h"

namespace FF
{
	const std::vector<std::unique_ptr<GraphNode>>& Graph::GetNodes() const
	{
		return m_pGraphNodes;
	}

	GraphNode* Graph::GetNode(int nodeId) const
	{
		if (!IsNodeValid(nodeId))
			return nullptr;

		return m_pGraphNodes[nodeId].get();
	}

	int Graph::AddNode(const Point2i& pos)
	{
		m_pGraphNodes.emplace_back(std::make_unique<GraphNode>(m_NextID, pos));
		m_pConnections.resize(m_NextID + 1);

		return m_NextID++;
	}

	void Graph::RemoveNode(unsigned nodeId)
	{
		if (!IsNodeValid(nodeId))
		{
			return;
		}

		//The to connections have to be cleared too
		for (auto& connVec : m_pConnections)
		{
			connVec.erase(std::remove_if(connVec.begin(), connVec.end(),
					[nodeId](const auto& connection) 
					{
						return connection->GetToNodeId() == nodeId;
					}
				),
				connVec.end()
			);
		}

		m_pGraphNodes[nodeId] = nullptr;
		m_pConnections[nodeId].clear();
	}

	bool Graph::IsNodeValid(int index) const
	{
		return (static_cast<size_t>(index) < m_pGraphNodes.size() && m_pGraphNodes[index] != nullptr);
	}

	void Graph::AddConnection(unsigned fromNodeId, unsigned toNodeId)
	{
		assert(IsNodeValid(fromNodeId) && IsNodeValid(toNodeId));

		m_pConnections[fromNodeId].emplace_back(std::make_unique<GraphConnection>(fromNodeId, toNodeId));
	}

	GraphConnection* Graph::GetConnection(unsigned fromNodeId, unsigned toNodeId) const
	{
		assert(IsNodeValid(fromNodeId));
		assert(IsNodeValid(toNodeId));

		for (const auto& connection : m_pConnections[fromNodeId])
		{
			if (connection && connection->GetToNodeId() == toNodeId)
			{
				return connection.get();
			}
		}

		return nullptr;
	}

	void Graph::RemoveConnection(unsigned fromNodeId, unsigned toNodeId)
	{
		auto connection{ GetConnection(fromNodeId, toNodeId) };

		for (auto it{begin(m_pConnections[fromNodeId])}; it != end(m_pConnections[fromNodeId]); ++it)
		{
			if (connection && connection->GetToNodeId() == toNodeId)
			{
				m_pConnections[fromNodeId].erase(it);
			}
		}
	}

	void Graph::RemoveConnection(GraphConnection* pConnection)
	{
		RemoveConnection(pConnection->GetFromNodeId(), pConnection->GetToNodeId() );
	}

	const std::vector<std::unique_ptr<GraphConnection>>& Graph::GetConnectionsFromNode(int nodeId) const
	{
		assert(IsNodeValid(nodeId));
		return m_pConnections[nodeId];
	}

	void Graph::DrawGraph() const
	{
		for (const auto& node : m_pGraphNodes)
		{
			if (node)
			{
				node->Draw();
			}
		}

		DrawConnections();
	}

	void Graph::DrawConnections() const
	{
		GAME_ENGINE->SetColor(RGB(0, 0, 0));

		for (size_t i{ 0 }; i < m_pConnections.size(); ++i)
		{
			for (const auto& pConn : m_pConnections[i])
			{
				const Point2i& pos1{ m_pGraphNodes[pConn->GetFromNodeId()]->GetPosition() };
				const Point2i& pos2{ m_pGraphNodes[pConn->GetToNodeId()]->GetPosition() };

				GAME_ENGINE->DrawLine(pos1.x, pos1.y, pos2.x, pos2.y);
			}
		}

	}
}