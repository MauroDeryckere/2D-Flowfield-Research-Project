#ifndef GRAPH 
#define GRAPH

#include "../Utils.h"

#include "GraphNode.h"
#include "GraphConnection.h"

#include <vector>
#include <memory>

using utils::Point2i;


//Graph needs to be updated; TODO

class Graph final
{
public:
	Graph() = default;
	~Graph() = default;

	void DrawGraph() const;

	//Nodes
	const std::vector<std::unique_ptr<GraphNode>>& GetNodes() const;

	GraphNode* GetNode(int nodeId) const;

	int AddNode(const Point2i& pos);
	void RemoveNode(unsigned nodeId);
	bool IsNodeValid(int index) const;

	//Connections
	void AddConnection(unsigned fromNodeId, unsigned toNodeId);
	GraphConnection* GetConnection(unsigned fromNodeId, unsigned toNodeId) const;
	void RemoveConnection(unsigned fromNodeId, unsigned toNodeId);
	void RemoveConnection(GraphConnection* pConnection);
	
	const std::vector<std::unique_ptr<GraphConnection>>& GetConnectionsFromNode(int nodeId) const;

private:
	std::vector<std::unique_ptr<GraphNode>> m_pGraphNodes;
	std::vector<std::vector<std::unique_ptr<GraphConnection>>> m_pConnections;

	unsigned m_NextID{ 0 };

private:
	void DrawConnections() const;
};

#endif
