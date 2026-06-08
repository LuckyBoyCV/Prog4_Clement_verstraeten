#include "ScoreDisplayComponent.h"
#include "QbertComponent.h"
#include "TextComponent.h"
#include "GameObject.h"
#include "GameEvent.h"
#include <string>

qbert::ScoreDisplayComponent::ScoreDisplayComponent(dae::GameObject* pOwner)
	: Component(pOwner)
{
}

void qbert::ScoreDisplayComponent::OnNotify(dae::GameEvent event, dae::GameObject* pOwner)
{
	if (event != dae::GameEvent::PlayerPickedUp) return;

	auto* qbert = pOwner->GetComponent<QbertComponent>();
	if (!qbert) return;

	auto* text = m_Owner->GetComponent<dae::TextComponent>();
	if (!text) return;

	text->SetText(std::to_string(qbert->getScore()));
}
