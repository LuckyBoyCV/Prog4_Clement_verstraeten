#include "mainMenuComponent.h"
#include "TextComponent.h"

qbert::mainMenuComponent::mainMenuComponent(dae::GameObject* pOwner,
	std::vector<dae::TextComponent*> items,
	std::function<void(gameMode)> onSelect)
	: Component(pOwner)
	, m_items(std::move(items))
	, m_onSelect(std::move(onSelect))
{
	updateColors();
}

void qbert::mainMenuComponent::SelectUp()
{
	m_selectedIndex = (m_selectedIndex - 1 + static_cast<int>(m_items.size())) % static_cast<int>(m_items.size());
	updateColors();
}

void qbert::mainMenuComponent::SelectDown()
{
	m_selectedIndex = (m_selectedIndex + 1) % static_cast<int>(m_items.size());
	updateColors();
}

void qbert::mainMenuComponent::Confirm()
{
	gameMode selectedMode;
	switch (m_selectedIndex)
	{
		case 0: selectedMode = gameMode::singlePlayer; break;
		case 1: selectedMode = gameMode::Coop;         break;
		case 2: selectedMode = gameMode::Versus;       break;
		default: return;
	}
	m_onSelect(selectedMode);
}

void qbert::mainMenuComponent::updateColors()
{
	for (int i = 0; i < static_cast<int>(m_items.size()); ++i)
	{
		if (i == m_selectedIndex)
			m_items[i]->SetColor({ 255, 165, 0, 255 });
		else
			m_items[i]->SetColor({ 180, 180, 180, 255 });
	}
}
