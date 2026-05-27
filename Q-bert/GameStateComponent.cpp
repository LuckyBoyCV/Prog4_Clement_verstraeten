#include "GameStateComponent.h"
#include "GameObject.h"
#include "GameEvent.h"

qbert::GameStateComponent::GameStateComponent(dae::GameObject* pOwner)
	: Component(pOwner)
{
}

void qbert::GameStateComponent::advanceRound()
{
	++m_round;
	m_subject.Notify(dae::GameEvent::roundChanged, m_Owner);
}

void qbert::GameStateComponent::advanceLevel()
{
	++m_level;
	m_round = 1;
	m_subject.Notify(dae::GameEvent::levelChanged, m_Owner);
}