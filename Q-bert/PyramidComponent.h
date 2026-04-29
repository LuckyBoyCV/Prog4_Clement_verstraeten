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
		void UpdateTileSprite(Tile& tile);

		glm::vec2 GetTileScreenPos(int row, int col) const;
		void SetTileType(Tile& tile, TileType type);

		static constexpr int   Rows = 7;
		static constexpr float TileW = 64.f;
		static constexpr float TileH = 32.f;
		static constexpr float StartX = 512.f;
		static constexpr float StartY = 80.f;

	private:
		Tile m_Tiles[Rows][Rows];
		std::shared_ptr<dae::Texture2D> m_tileTexture;
	};
}