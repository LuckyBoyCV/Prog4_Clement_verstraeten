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
		Tile* getTile(int row, int col);
		bool stepOn(Tile& tile);        // returns true if the step scored advanced the tile
		bool isStepped() const;
		void reset(); // clears every tile back to empty used on round advance
		void setFlipRule(TileType type); // re-types every non-disc tile to this flip rule set per level from levels.json
		void updateTileSprite(Tile& tile);
		void reverseStep(Tile& tile);

		glm::vec2 getTileScreenPos(int row, int col) const;
		void setTileType(Tile& tile, TileType type);

		// Flying discs that sit just off the left/right edges. (Re)placed each round.
		void placeDisks();
		int getDiskIndexAt(int row, int col) const; // active disc at (row,col), or -1
		Disk& getDisk(int index);
		const Disk& getDisk(int index) const;
		void consumeDisk(int index);                  // mark a ridden disc inactive

		// Tile colour set: index into the sprite sheet's columns of cube colours.
		// The pyramid tiles and the CHANGE TO swatch both use it; it changes per round.
		int getColorSet() const { return m_colorSet; }
		void setColorSet(int set);

		// The state a tile must reach to count as flipped this level (drives the CHANGE
		// TO swatch and round completion): visitedTwice on level 2, otherwise visited.
		TileState getTargetState() const;

		static constexpr int   colorSetCount  = 8;   // usable colour sets 
		static constexpr int   colorSetOffset = 80;  // px between colour sets on the sheet

		static constexpr int   Rows = 7;
		static constexpr float TileW = 90.f;
		static constexpr float TileH = 60.f;
		static constexpr float StartX = 450.f;
		static constexpr float StartY = 144.f;

		// Row the two discs sit on; the player mounts them from the row below's edge tiles.
		static constexpr int   DiskRow = 3;

	private:
		Tile m_Tiles[Rows][Rows];
		std::vector<Disk> m_Disks;
		std::shared_ptr<dae::Texture2D> m_tileTexture;
		int m_colorSet{ 0 };
	};
}