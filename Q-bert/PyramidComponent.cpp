#include "PyramidComponent.h"
#include "Renderer.h"
#include <SDL3/SDL.h>
#include "ResourceManager.h"

namespace
{
	// The state a tile must reach for the round to count as complete.
	TileState TargetStateFor(TileType type)
	{
		return (type == TileType::twice) ? TileState::visitedTwice : TileState::visited;
	}

	// Source rect of the disc sprite on sprites_Qbert.png. ESTIMATED from the sheet (the
	// disc sits by the "Rotate disk colors" note, ~3rd sprite row); tweak to pixel-fit.
	// The arcade disc also colour-cycles through several frames if you want to animate it.
	constexpr float DiskSrcX = 176.f;
	constexpr float DiskSrcY = 72.f;
	constexpr float DiskSrcW = 32.f;
	constexpr float DiskSrcH = 32.f;
}

qbert::PyramidComponent::PyramidComponent(dae::GameObject* pOwner)
	: Component(pOwner),
	m_Tiles{},
	m_tileTexture(nullptr)
{
	
	m_tileTexture = dae::ResourceManager::GetInstance().LoadTexture("sprites_Qbert.png");

	for (int r = 0; r < Rows; ++r)
	{
		for (int c = 0; c <= r; ++c)
		{
			auto& tile = m_Tiles[r][c];
			tile.row = r;
			tile.col = c;
			tile.state = TileState::empty;
			tile.position = GetTileScreenPos(r, c);
			UpdateTileSprite(tile);

		}
	}

	PlaceDisks();
}

Tile* qbert::PyramidComponent::GetTile(int row, int col)
{
	if (row < 0 || row >= Rows || col < 0 || col > row)
		return nullptr;
	return &m_Tiles[row][col];
}

bool qbert::PyramidComponent::StepOn(Tile& tile)
{
	const TileState before = tile.state;

	switch (tile.type)
	{
	case TileType::normal:
		tile.state = TileState::visited;
		break;

	case TileType::twice:
		// Level 2: needs two hops, empty -> visited -> visitedTwice (then it stays).
		if (tile.state == TileState::empty)
			tile.state = TileState::visited;
		else if (tile.state == TileState::visited)
			tile.state = TileState::visitedTwice;
		break;

	case TileType::reverse:
		// Level 3: each hop toggles, so stepping on a finished tile reverts it.
		tile.state = (tile.state == TileState::visited)
			? TileState::empty
			: TileState::visited;
		break;

	case TileType::disk:
		break;
	}

	UpdateTileSprite(tile);

	// Score only when the tile advances toward its target colour, so level-3
	// reverts and re-hopping an already-finished tile award nothing.
	return static_cast<int>(tile.state) > static_cast<int>(before);
}

bool qbert::PyramidComponent::IsStepped() const
{
	// Round is complete once every tile (disks aside) sits on its target state.
	// That target depends on the tile's flip rule: visitedTwice for level 2,
	// visited for levels 1 and 3.
	for (int row = 0; row < Rows; ++row)
	{
		for (int col = 0; col <= row; ++col)
		{
			const Tile& tile = m_Tiles[row][col];
			if (tile.type == TileType::disk)
				continue;
			if (tile.state != TargetStateFor(tile.type))
				return false;
		}
	}
	return true;
}

void qbert::PyramidComponent::Reset()
{
	for (int r = 0; r < Rows; ++r)
	{
		for (int c = 0; c <= r; ++c)
		{
			auto& tile = m_Tiles[r][c];
			tile.state = TileState::empty;
			UpdateTileSprite(tile);
		}
	}
}

TileState qbert::PyramidComponent::getTargetState() const
{
	// Every tile shares the level's flip rule, so the apex's type is representative.
	return TargetStateFor(m_Tiles[0][0].type);
}

void qbert::PyramidComponent::setFlipRule(TileType type)
{
	// Re-type every tile (discs excepted) so StepOn/IsStepped follow this level's flip rule.
	for (int r = 0; r < Rows; ++r)
		for (int c = 0; c <= r; ++c)
			if (m_Tiles[r][c].type != TileType::disk)
				m_Tiles[r][c].type = type;
}

void qbert::PyramidComponent::setColorSet(int set)
{
	m_colorSet = set;
	for (int r = 0; r < Rows; ++r)
		for (int c = 0; c <= r; ++c)
			UpdateTileSprite(m_Tiles[r][c]);
}

void qbert::PyramidComponent::UpdateTileSprite(Tile& tile)
{
	int stateOffset = 32;
	switch (tile.state)
	{
	case TileState::empty:       
		stateOffset = 0; 
		break;
	case TileState::visited:     
		stateOffset = 32; 
		break;
	case TileState::visitedTwice: 
		stateOffset = 64; 
		break;
	}

	tile.spriteX = m_colorSet * ColorSetStride;
	tile.spriteY = 160+stateOffset;
	tile.spriteW = 32;
	tile.spriteH = 32;
}

void qbert::PyramidComponent::ReverseStep(Tile& tile)
{
	if (tile.state == TileState::visited)
		tile.state = TileState::empty;
	else if (tile.state == TileState::visitedTwice)
		tile.state = TileState::visited;
	
	UpdateTileSprite(tile);
}

glm::vec2 qbert::PyramidComponent::GetTileScreenPos(int row, int col) const
{
	float x = StartX + (col - row * 0.5f) * TileW;
	float y = StartY + row * (TileH - 20.f);
	return { x,y };
};

void qbert::PyramidComponent::SetTileType(Tile& tile, TileType type)
{
	tile.type = type;
}

void qbert::PyramidComponent::PlaceDisks()
{
	// Two discs per board: one off the left edge (col -1), one off the right (col row+1),
	// both on DiskRow. Rebuilt every round so consumed discs come back.
	m_Disks.clear();

	Disk left{};
	left.row = DiskRow;
	left.col = -1;
	left.position = GetTileScreenPos(left.row, left.col);
	m_Disks.push_back(left);

	Disk right{};
	right.row = DiskRow;
	right.col = DiskRow + 1;
	right.position = GetTileScreenPos(right.row, right.col);
	m_Disks.push_back(right);
}

int qbert::PyramidComponent::GetDiskIndexAt(int row, int col) const
{
	for (int i = 0; i < static_cast<int>(m_Disks.size()); ++i)
	{
		const Disk& disk = m_Disks[i];
		if (disk.active && disk.row == row && disk.col == col)
			return i;
	}
	return -1;
}

Disk& qbert::PyramidComponent::GetDisk(int index)
{
	return m_Disks[index];
}

const Disk& qbert::PyramidComponent::GetDisk(int index) const
{
	return m_Disks[index];
}

void qbert::PyramidComponent::ConsumeDisk(int index)
{
	if (index >= 0 && index < static_cast<int>(m_Disks.size()))
		m_Disks[index].active = false;
}

void qbert::PyramidComponent::Render()
{
	auto& renderer = dae::Renderer::GetInstance();

	for (int r = 0; r < Rows; ++r)
	{
		for (int c = 0; c <= r; ++c)
		{
			const auto& tile = m_Tiles[r][c];

			SDL_FRect src{ static_cast<float>(tile.spriteX), static_cast<float>(tile.spriteY),
						   static_cast<float>(tile.spriteW), static_cast<float>(tile.spriteH) };

			SDL_FRect dst{ tile.position.x, tile.position.y, TileW, TileH };

			SDL_RenderTexture(renderer.GetSDLRenderer(),
				m_tileTexture->GetSDLTexture(), &src, &dst);
		}
	}

	// Discs sit beside the pyramid, drawn after the tiles.
	for (const auto& disk : m_Disks)
	{
		if (!disk.active)
			continue;

		SDL_FRect src{ DiskSrcX, DiskSrcY, DiskSrcW, DiskSrcH };
		SDL_FRect dst{ disk.position.x, disk.position.y, TileW, TileH };
		SDL_RenderTexture(renderer.GetSDLRenderer(),
			m_tileTexture->GetSDLTexture(), &src, &dst);
	}
}