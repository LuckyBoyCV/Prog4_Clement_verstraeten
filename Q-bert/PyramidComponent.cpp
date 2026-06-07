#include "PyramidComponent.h"
#include "Renderer.h"
#include <SDL3/SDL.h>
#include "ResourceManager.h"

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
}

Tile* qbert::PyramidComponent::GetTile(int row, int col)
{
	if (row < 0 || row >= Rows || col < 0 || col > row)
		return nullptr;
	return &m_Tiles[row][col];
}

void qbert::PyramidComponent::StepOn(Tile& tile)
{
	switch (tile.type)
	{
	case::TileType::normal:
			tile.state = TileState::visited;
			break;

	case::TileType::twice:
		if (tile.state==TileState::empty)
		{
			tile.state = TileState::visited;
		}
		else if (tile.state == TileState::visited)
			tile.state = TileState::visitedTwice;
		break;

	case TileType::reverse:
		if (tile.state == TileState::visitedTwice)
		{
			tile.state = TileState::visited;
		}
		else if (tile.state == TileState::visited)
		{
			tile.state = TileState::empty;
		}
		else
		{
			tile.state = TileState::visitedTwice;
		}
		break;

		case TileType::disk:
			
			break;


	}
	UpdateTileSprite(tile);

}

bool qbert::PyramidComponent::IsStepped() const
{
	for (size_t row = 0; row < Rows; row++)
	{
		for (size_t col = 0; col <= row; col++)
		{
			if (m_Tiles[row][col].state==TileState::empty)
			{
				return false;
			}
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

	tile.spriteX = 0;
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
}