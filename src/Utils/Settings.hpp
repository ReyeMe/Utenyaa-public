#pragma once

#include <stddef.h>

#include "..\Entities\World.hpp"

struct Settings
{
    static constexpr inline size_t MaxPlayerCount = 4;
    static constexpr inline size_t StageCount = 4;
    static inline const char* StageNames[StageCount] = { "Island", "Cross", "Valley" , "Railway" };
    static inline const char* StageFiles[StageCount] = { "ISLAND.UTE", "CROSS.UTE", "VALLEY.UTE", "RAILWAY.UTE" };
    static inline size_t SelectedStage = 0;
    static inline size_t PlayerCount = 2;
    static inline size_t TotalSeconds = 120;
    static inline bool IsActive = false;
    static inline bool GameEnded = false;
    static inline bool Quit = false;

	// Updated after game ends
	static inline int PlayerWon = 1;
};
