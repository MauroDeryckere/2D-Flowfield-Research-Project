#include "Graph/GraphConnection.h"
#include "GameEngine.h"

GraphConnection::GraphConnection(int fromId, int toId) :
    m_FromId{ fromId },
	m_ToId{ toId }
{

}

int GraphConnection::GetFromNodeId() const
{
	return m_FromId;
}

void GraphConnection::SetFromNodeId(unsigned id)
{
	m_FromId = id;
}

int GraphConnection::GetToNodeId() const
{
	return m_ToId;
}

void GraphConnection::SetToNodeId(unsigned id)
{
	m_ToId = id;
}

bool GraphConnection::IsValid() const
{
	return (m_FromId != -1 && m_ToId != -1);
}
