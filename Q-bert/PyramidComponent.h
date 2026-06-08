#pragma once
#include "Component.h"
#include "Tile.h"
#include <vector>
#include <glm/glm.hpp>
#include "Texture2D.h"
#include <memory>

namespace dae { 
	
	class Renderer;

}
namespace qbert
{
	class PyramidComponent final : public dae::Component
	{
	public:
		explicit PyramidComponent(dae::GameObject* pOwner);
		~PyramidComponent() override = default;
		PyramidComponent(const PyramidComponent&) = delete;
		PyramidComponent(PyramidComponent&&) = delete;
		PyramidComponent& operator=(const PyramidComponent&) = delete;
		PyramidComponent& operator=(PyramidComponent&&) = delete;
		
		void Render() override;
		Tile* GetTile(int row, int col);
		void  StepOn(Tile& tile);
		bool  IsStepped() const;
		void  Reset(); // clears every tile back to empty (used on round advance)
		void UpdateTileSprite(Tile& tile);
		void ReverseStep(Tile& tile);

		glm::vec2 GetTileScreenPos(int row, int col) const;
		void SetTileType(Tile& tile, TileType type);

		// Tile colour set: index into the sprite sheet's columns of cube colours.
		// The pyramid tiles and the CHANGE TO swatch both use it; it changes per round.
		int  getColorSet() const { return m_colorSet; }
		void setColorSet(int set);

		static constexpr int   ColorSetCount  = 8;   // usable colour sets (set 8 is black, excluded)
		static constexpr int   ColorSetStride = 80;  // px between colour sets on the sheet

		static constexpr int   Rows = 7;
		static constexpr float TileW = 90.f;
		static constexpr float TileH = 60.f;
		static constexpr float StartX = 450.f;
		static constexpr float StartY = 144.f;

	private:
		Tile m_Tiles[Rows][Rows];
		std::shared_ptr<dae::Texture2D> m_tileTexture;
		int m_colorSet{ 0 };
	};
}