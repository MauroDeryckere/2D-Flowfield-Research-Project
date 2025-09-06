#ifndef FF_DEFINES_H
#define	FF_DEFINES_H

namespace FF
{
	// Visualization
	inline bool g_DrawCostField{ false };
	inline bool g_DrawIntegrationField{ true };
	inline bool g_DrawFlowField{ true };

	inline bool g_DrawPortals{ true };
	inline bool g_DrawGraph{ false };

	// Initialization
	inline constexpr bool g_InitRandomCostField{ false };
	inline constexpr uint32_t AMT_OF_AGENTS{ 1 };
}

#endif