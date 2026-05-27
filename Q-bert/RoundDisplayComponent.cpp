#include "RoundDisplayComponent.h"
#include "GameStateComponent.h"
#include "TextComponent.h"
#include "GameObject.h"
#include "GameEvent.h"
#include "ResourceManager.h"
#include "Renderer.h"

qbert::RoundDisplayComponent::RoundDisplayComponent(dae::GameObject* pOwner, dae::GameObject* pTextObject)
	: Component(pOwner), 
	m_pTextObject(pTextObject)
{
	m_spriteSheet = dae::ResourceManager::GetInstance().LoadTexture("sprites_Qbert.png");
}

void qbert::RoundDisplayComponent::Render()
{
	if (!m_spriteSheet) return;

	const glm::vec3 pos = m_Owner->GetWorldPosition();
	constexpr float scale = 3.f;

	SDL_FRect dst{ pos.x - srcRect.w * scale, pos.y +10.f,
				   srcRect.w * scale, srcRect.h * scale };

	SDL_RenderTexture(
		dae::Renderer::GetInstance().GetSDLRenderer(),
		m_spriteSheet->GetSDLTexture(),
		&srcRect, &dst);
}

void qbert::RoundDisplayComponent::OnNotify(dae::GameEvent event, dae::GameObject* pOwner)
{
	if (event != dae::GameEvent::roundChanged && event != dae::GameEvent::levelChanged)
		return;

	auto* state = pOwner->GetComponent<GameStateComponent>();
	if (!state) return;

	auto* text = m_pTextObject->GetComponent<dae::TextComponent>();

	text->SetText(std::to_string(state->getRound()));
}