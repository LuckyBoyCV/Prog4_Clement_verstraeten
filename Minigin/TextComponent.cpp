#include "TextComponent.h"
#include "GameObject.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "RenderComponent.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <stdexcept>

dae::TextComponent::TextComponent(GameObject* pOwner, const std::string& text, std::shared_ptr<Font> font)
	: Component(pOwner)
	, m_Text(text)
	, m_pFont(font)
	, m_renderComponent(pOwner)
{
	
}

void dae::TextComponent::Update(float )
{
	if(m_NeedsUpdate)
	{
		
		regerateTexture();
		m_NeedsUpdate = false;
	}
}

void dae::TextComponent::Render() const
{
	m_renderComponent.Render();
}

void dae::TextComponent::SetText(const std::string& text)
{
	if(m_Text != text)
	{
		m_Text = text;
		m_NeedsUpdate = true;
	}
}

void dae::TextComponent::SetColor(SDL_Color color)
{
		m_Color = color;
		m_NeedsUpdate = true;
	
}

void dae::TextComponent::regerateTexture()
{
	if (m_Text.empty()) return;

	SDL_Surface* surface = TTF_RenderText_Blended(
		m_pFont->GetFont(), m_Text.c_str(), m_Text.size(), m_Color);

	if (!surface)
		throw std::runtime_error(std::string("TextComponent: ") + SDL_GetError());

	SDL_Texture* sdlTex = SDL_CreateTextureFromSurface(
		Renderer::GetInstance().GetSDLRenderer(), surface);

	SDL_DestroySurface(surface);

	if (!sdlTex)
		throw std::runtime_error(std::string("TextComponent: ") + SDL_GetError());

	m_renderComponent.SetTexture(std::make_shared<Texture2D>(sdlTex));
}