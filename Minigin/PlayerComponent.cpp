#include "PlayerComponent.h"
#include "GameObject.h"

dae::PlayerComponent::PlayerComponent(GameObject* owner, int lives)
    : Component(owner)
    , m_Lives(lives)
{
}

void dae::PlayerComponent::LoseLife()
{
    if (m_Lives > 0)
        --m_Lives;
    m_subject.Notify(GameEvent::PlayerDied, m_Owner);
}

void dae::PlayerComponent::AddScore(int score)
{
    m_Score += score;
    m_subject.Notify(GameEvent::PlayerPickedUp, m_Owner);
}