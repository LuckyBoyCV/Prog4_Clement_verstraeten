#include "gameOverComponent.h"
#include "QbertComponent.h"
#include "GameEvent.h"
#include <algorithm>
#include <utility>

qbert::gameOverComponent::gameOverComponent(dae::GameObject* pOwner, std::vector<QbertComponent*> qberts,
	std::function<void(int)> onGameOver)
	: Component(pOwner)
	, m_qberts(std::move(qberts))
	, m_onGameOver(std::move(onGameOver))
{
}

void qbert::gameOverComponent::OnNotify(dae::GameEvent event, dae::GameObject*)
{
	if (event != dae::GameEvent::GameOver || m_triggered)
		return;

	m_triggered = true;

	// the win path notifies from the game state object which has no qbert on it, so read the
	// score straight off the players we were handed rather than off the notifier
	int best = 0;
	for (auto* qbert : m_qberts)
		best = std::max(best, qbert->getScore());

	if (m_onGameOver)
		m_onGameOver(best);
}
