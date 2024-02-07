#ifndef GRAPHNODE
#define GRAPHNODE

#include "Utils.h"

using utils::Point2i;

class GraphNode final
{
public:
	GraphNode(int id, const Point2i position = {}) : m_Position{ position }, m_Id{ id } {}
	~GraphNode() = default;

	void Draw() const;

	int GetId() const { return m_Id; }

	const Point2i& GetPosition() const { return m_Position; }
	void SetPosition(const Point2i& newPos) { m_Position = newPos; }

private:
	int m_Id;
	Point2i m_Position;
};

#endif
