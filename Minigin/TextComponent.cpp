#include "TextComponent.h"
#include "GameObject.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <stdexcept>

dae::TextComponent::TextComponent(GameObject* pOwner, const std::string& text, std::shared_ptr<Font> font)
	: Component(pOwner)
	, m_Text(text)
	, m_pFont(font)
{

}

void dae::TextComponent::Update(float deltatime)
{
	if(m_NeedsUpdate)
	{
		(void)deltatime;
		regerateTexture();
		m_NeedsUpdate = false;
	}
}

void dae::TextComponent::Render() const
{
	if (!m_pTexture)
		return;

	const auto& pos = m_Owner->GetTransform().GetPosition();
	Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
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

	m_pTexture = std::make_unique<Texture2D>(sdlTex);
}