#ifndef PORTAL
#define	PORTAL

#include <vector>
#include "../../Utils.h"

using utils::Point2i;

struct Portal
{
	enum class PortalSide
	{
		LEFT,
		RIGHT,
		TOP,
		BOTTOM
	};

	Point2i portalPos;
	int portalId = - 1;

	std::vector<Point2i> FromFieldwindowPostions;
	std::vector<unsigned> toFieldWindowCellIdxes;

	unsigned toFieldId; 
	unsigned fromFieldId;

	Portal() = default;

	Portal(const Point2i& pos, unsigned pId, const std::vector<Point2i>& FFieldwindowPos, const std::vector<unsigned>& toFWindowIdxs, unsigned toFId, unsigned fromFId) :
		portalPos{ pos },
		portalId{static_cast<int>(pId)},
		FromFieldwindowPostions{ FFieldwindowPos },
		toFieldWindowCellIdxes{ toFWindowIdxs },
		toFieldId{ toFId },
		fromFieldId{ fromFId }
	{
	};
};

#endif

