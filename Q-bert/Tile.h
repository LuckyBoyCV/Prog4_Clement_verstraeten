#pragma once
#include <glm/glm.hpp>

enum class TileType
{
	normal,
	twice,
	reverse,
	disk
};

enum class TileState
{
	empty,
	visited,
	visitedTwice
};

struct Tile
{
	int row{};
	int col{};
	TileState state{TileState::empty};
	TileType type{ TileType::normal };
	glm::vec2 position{};


	int spriteX{ 0 };
	int spriteY{ 160 };
	int spriteW{ 32 };
	int spriteH{ 32 };
};

// A flying disc that floats just off the left/right edge of the pyramid. Q*bert can jump
// onto one to ride up to the apex; it lives outside the m_Tiles triangle (col -1 or row+1).
struct Disk
{
	int row{};            // off-pyramid coord (matches the jump target that lands on it)
	int col{};            // left disc = -1, right disc = row + 1
	glm::vec2 position{}; // screen pos from PyramidComponent::GetTileScreenPos
	bool active{ true };  // consumed discs go inactive until the round refreshes
};