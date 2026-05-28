#pragma once
#include "Component.h"
#include "gameMode.h"
#include <functional>
#include <vector>

namespace dae { class TextComponent; }

namespace qbert
{
	class mainMenuComponent final : public dae::Component
	{
	public:
		explicit mainMenuComponent(dae::GameObject* pOwner,std::vector<dae::TextComponent*> items,std::function<void(gameMode)> onSelect);
		~mainMenuComponent() override = default;
		mainMenuComponent(const mainMenuComponent&) = delete;
		mainMenuComponent(mainMenuComponent&&) = delete;
		mainMenuComponent& operator=(const mainMenuComponent&) = delete;
		mainMenuComponent& operator=(mainMenuComponent&&) = delete;

		void Update(float deltaTime) override;

	private:
		void updateColors();

		std::vector<dae::TextComponent*> m_items;
		std::function<void(gameMode)> m_onSelect; // called with the chosen mode on confirm
		int m_selectedIndex{ 0 };

		// Edge-detection: only react on the frame the key goes down
		bool m_upWasDown{ false };
		bool m_downWasDown{ false };
		bool m_enterWasDown{ false };
	};
}
