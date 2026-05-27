#pragma once
#include <SDL3/SDL.h>
#include "Component.h"
#include "Observer.h"
#include "Texture2D.h"
#include <memory>

namespace qbert
{
	class LevelDisplayComponent final : public dae::Component, public dae::Observer
	{
	public:
		explicit LevelDisplayComponent(dae::GameObject* pOwner, dae::GameObject* pTextObject);
		~LevelDisplayComponent() override = default;

		LevelDisplayComponent(const LevelDisplayComponent&) = delete;
		LevelDisplayComponent(LevelDisplayComponent&&) = delete;
		LevelDisplayComponent& operator=(const LevelDisplayComponent&) = delete;
		LevelDisplayComponent& operator=(LevelDisplayComponent&&) = delete;

		void Render() override;
		void OnNotify(dae::GameEvent event, dae::GameObject* pOwner) override;

	private:
		std::shared_ptr<dae::Texture2D> m_spriteSheet;
		dae::GameObject* m_pTextObject;
		
		static constexpr SDL_FRect srcRect{ 240.f, 130.f, 32.f, 8.f };
	};
}