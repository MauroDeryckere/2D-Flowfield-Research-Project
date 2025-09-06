#ifndef FF_GRAPHCONNECTION_H
#define FF_GRAPHCONNECTION_H

namespace FF
{
	class GraphConnection final
	{
	public:
		GraphConnection(int fromId = -1, int toId = -1);
		~GraphConnection() = default;

		int GetFromNodeId() const;
		void SetFromNodeId(unsigned id);

		int GetToNodeId() const;
		void SetToNodeId(unsigned id);

		bool IsValid() const;

	private:
		int m_FromId;
		int m_ToId;
	};
}

#endif