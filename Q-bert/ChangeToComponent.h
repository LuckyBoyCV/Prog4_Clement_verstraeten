#pragma once
#include <SDL3/SDL.h>
#include "Component.h"
#include "Texture2D.h"
#include <memory>

namespace qbert
{
	class PyramidComponent;

	// Renders the target-colour tile swatch (the small cube from the sprite sheet)
	// beneath a plain-text "CHANGE TO:" label. The swatch follows the pyramid's
	// current colour set, which is randomised each round.
	class ChangeToComponent final : public dae::Component
	{
	public:
		ChangeToComponent(dae::GameObject* pOwner, PyramidComponent* pyramid);
		~ChangeToComponent() override = default;

		ChangeToComponent(const ChangeToComponent&) = delete;
		ChangeToComponent(ChangeToComponent&&) = delete;
		ChangeToComponent& operator=(const ChangeToComponent&) = delete;
		ChangeToComponent& operator=(ChangeToComponent&&) = delete;

		void Render() override;

	private:
		PyramidComponent* m_pPyramid;
		std::shared_ptr<dae::Texture2D> m_spriteSheet;

		// Visited-state mini cube on the sheet: x = swatchBaseX + colourSet * stride.
		static constexpr int   swatchBaseX  = 65;
		static constexpr int   swatchStride = 80;
		static constexpr float swatchY      = 196.f;
		static constexpr float swatchW      = 14.f;
		static constexpr float swatchH      = 12.f;
	};
}
