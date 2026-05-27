#include "mainMenuComponent.h"
#include "TextComponent.h"
#include <SDL3/SDL.h>

qbert::mainMenuComponent::mainMenuComponent(dae::GameObject* pOwner,
	std::vector<dae::TextComponent*> items,
	std::function<void(gameMode)> onSelect)
	: Component(pOwner)
	, m_items(std::move(items))
	, m_onSelect(std::move(onSelect))
{
	updateColors();
}

void qbert::mainMenuComponent::Update(float)
{
	const bool* keys = SDL_GetKeyboardState(nullptr);

	const bool upDown    = keys[SDL_SCANCODE_UP]     != 0;
	const bool downDown  = keys[SDL_SCANCODE_DOWN]   != 0;
	const bool enterDown = keys[SDL_SCANCODE_RETURN] != 0;

	if (upDown && !m_upWasDown)
	{
		m_selectedIndex = (m_selectedIndex - 1 + static_cast<int>(m_items.size())) % static_cast<int>(m_items.size());
		updateColors();
	}
	if (downDown && !m_downWasDown)
	{
		m_selectedIndex = (m_selectedIndex + 1) % static_cast<int>(m_items.size());
		updateColors();
	}
	if (enterDown && !m_enterWasDown)
	{
		gameMode selectedMode;
		switch (m_selectedIndex)
		{
			case 0: selectedMode = gameMode::singlePlayer; break;
			case 1: selectedMode = gameMode::Coop; break;
			case 2: selectedMode = gameMode::Versus; break;
			default: return;
		}
		m_onSelect(selectedMode);
	}

	m_upWasDown    = upDown;
	m_downWasDown  = downDown;
	m_enterWasDown = enterDown;
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
