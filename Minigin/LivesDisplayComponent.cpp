#include "LivesDisplayComponent.h"
#include "PlayerComponent.h"
#include "TextComponent.h"
#include "GameObject.h"
#include <string>

dae::LivesDisplayComponent::LivesDisplayComponent(GameObject* owner)
    : Component(owner)
{
}

void dae::LivesDisplayComponent::OnNotify(dae::GameObject* pOwner)
{
    auto* player = pOwner->GetComponent<PlayerComponent>();
    if (!player) return;

    auto* text = m_Owner->GetComponent<TextComponent>();
    if (!text) return;

    text->SetText("# lives: " + std::to_string(player->GetLives()));
}