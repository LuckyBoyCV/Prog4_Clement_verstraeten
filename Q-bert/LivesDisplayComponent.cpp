#include "LivesDisplayComponent.h"
#include "QbertComponent.h"
#include "TextComponent.h"
#include "GameObject.h"
#include "GameEvent.h"
#include <string>

qbert::LivesDisplayComponent::LivesDisplayComponent(dae::GameObject* pOwner)
	: Component(pOwner)
{
}

void qbert::LivesDisplayComponent::OnNotify(dae::GameEvent event, dae::GameObject* pOwner)
{
	// Both events are raised by QbertComponent::loseLife and lower the life count.
	if (event != dae::GameEvent::PlayerDied && event != dae::GameEvent::GameOver)
		return;

	auto* qbert = pOwner->GetComponent<QbertComponent>();
	if (!qbert) return;

	auto* text = m_Owner->GetComponent<dae::TextComponent>();
	if (!text) return;

	text->SetText("Lives: " + std::to_string(qbert->getLives()));
}
