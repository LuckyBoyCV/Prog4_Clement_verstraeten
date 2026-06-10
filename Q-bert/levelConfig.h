#pragma once
#include "Tile.h"          // TileType
#include <vector>
#include <filesystem>

namespace qbert
{
	// One level's rules, loaded from Data/levels.json. flipRule decides how a tile advances
	// when stepped on (see PyramidComponent::StepOn); colorSet pins the tile colour for the
	// whole level, or -1 to keep the original random-per-round behaviour.
	struct levelData
	{
		TileType flipRule{ TileType::normal };
		int      colorSet{ -1 };   // -1 = random each round, 0..ColorSetCount-1 = fixed
	};

	// The whole level table. The number of levels is simply levels.size().
	struct levelConfig
	{
		int                    roundsPerLevel{ 4 };
		std::vector<levelData> levels;
	};

	// Reads the level table from jsonPath. On a missing or malformed file it logs and returns
	// the built-in default (the classic 3-level layout), so the game always has at least one
	// valid level and never crashes on bad data.
	levelConfig loadLevelConfig(const std::filesystem::path& jsonPath);
}
