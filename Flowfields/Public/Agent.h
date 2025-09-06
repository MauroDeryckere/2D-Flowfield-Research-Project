#ifndef FF_AGENT_H
#define FF_AGENT_H

#include "Utils.h"
using utils::Point2i;

namespace FF
{
	class Agent final
	{
	public:
		Agent(const Point2i& pos);
		~Agent() = default;

		Agent(const Agent&) = delete;
		Agent& operator=(const Agent&) = delete;
		Agent(Agent&&) noexcept = delete;
		Agent& operator=(Agent&&) noexcept = delete;

		void Render() const;
		void Update(class Grid* pGrid);

	private:
		Point2i m_Pos;
		const unsigned m_AgentSize{ 20 };
	};
}
class GridSector;



#endif
