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