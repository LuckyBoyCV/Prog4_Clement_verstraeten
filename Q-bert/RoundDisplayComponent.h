#pragma once
#include <SDL3/SDL.h>
#include "Component.h"
#include "Observer.h"
#include "Texture2D.h"
#include <memory>

namespace qbert
{
	class RoundDisplayComponent final : public dae::Component, public dae::Observer
	{
	public:
		explicit RoundDisplayComponent(dae::GameObject* pOwner, dae::GameObject* pTextObject);
		~RoundDisplayComponent() override = default;

		RoundDisplayComponent(const RoundDisplayComponent&) = delete;
		RoundDisplayComponent(RoundDisplayComponent&&) = delete;
		RoundDisplayComponent& operator=(const RoundDisplayComponent&) = delete;
		RoundDisplayComponent& operator=(RoundDisplayComponent&&) = delete;

		void Render() override;
		void OnNotify(dae::GameEvent event, dae::GameObject* pOwner) override;

	private:
		std::shared_ptr<dae::Texture2D> m_spriteSheet;
		dae::GameObject* m_pTextObject;

		static constexpr SDL_FRect srcRect{ 240.f, 135.f, 32.f, 8.f };
	};
}