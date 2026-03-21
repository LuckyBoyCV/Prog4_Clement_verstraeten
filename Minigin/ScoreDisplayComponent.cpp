#include "ScoreDisplayComponent.h"
#include "PlayerComponent.h"
#include "TextComponent.h"
#include "GameObject.h"
#include <string>

dae::ScoreDisplayComponent::ScoreDisplayComponent(GameObject* owner)
    : Component(owner)
{
}

void dae::ScoreDisplayComponent::OnNotify(GameEvent event, GameObject* pOwner)
{
    if (event != GameEvent::PlayerPickedUp) return;

    auto* player = pOwner->GetComponent<PlayerComponent>();
    if (!player) return;

    auto* text = m_Owner->GetComponent<TextComponent>();
    if (!text) return;

    text->SetText("Score: " + std::to_string(player->GetScore()));
}