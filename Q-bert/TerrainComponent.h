#pragma once
#include "Component.h"

namespace dae
{
	class RenderComponent;
}

namespace qbert
{
	enum class TileState
	{
		Normal,
		Double,
		reversed,
		Disc
	};

	class TerrainComponent final : public dae::Component
	{
		public:

		TerrainComponent(dae::GameObject* owner, TileState state , dae::RenderComponent* pPlatform);
		 ~TerrainComponent() override = default;
		 void initialize() override;
		 void Update(float) override;
		 void Render() override;
		TerrainComponent(const TerrainComponent&) = delete;
		TerrainComponent(TerrainComponent&&) = delete;
		TerrainComponent& operator=(const TerrainComponent&) = delete;
		TerrainComponent& operator=(TerrainComponent&&) = delete;
	};
}
