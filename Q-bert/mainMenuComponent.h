#pragma once
#include "Component.h"
#include "gameMode.h"
#include <functional>
#include <vector>

namespace dae { class TextComponent; }

namespace qbert
{
	class MainMenuComponent final : public dae::Component
	{
	public:
		explicit MainMenuComponent(dae::GameObject* pOwner, std::vector<dae::TextComponent*> modes, std::function<void(gameMode)> onSelect);
		~MainMenuComponent() override = default;
		MainMenuComponent(const MainMenuComponent&) = delete;
		MainMenuComponent(MainMenuComponent&&) = delete;
		MainMenuComponent& operator=(const MainMenuComponent&) = delete;
		MainMenuComponent& operator=(MainMenuComponent&&) = delete;

		void SelectUp();
		void SelectDown();
		void Confirm();

	private:
		void updateColors();

		std::vector<dae::TextComponent*> m_items;
		std::function<void(gameMode)> m_onSelect;
		int m_selectedIndex{ 0 };
	};
}
