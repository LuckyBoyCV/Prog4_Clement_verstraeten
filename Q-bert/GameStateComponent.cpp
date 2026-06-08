#include "GameStateComponent.h"
#include "GameObject.h"
#include "GameEvent.h"
#include "PyramidComponent.h"
#include "QbertComponent.h"
#include <cstdlib>

qbert::GameStateComponent::GameStateComponent(dae::GameObject* pOwner)
	: Component(pOwner)
{
}

void qbert::GameStateComponent::Update(float)
{
	// Round is complete once every tile has been flipped. Advancing resets the
	// pyramid (so IsStepped() is false again next frame, no double-trigger) and
	// sends Q*bert back to the start. advanceRound() notifies roundChanged, which
	// the enemySpawnerComponent uses to re-gate enemies for the new round.
	if (m_pPyramid && m_pPyramid->IsStepped())
	{
		advanceRound();

		// New round -> new random target colour (always different from the current
		// one). The pyramid tiles and the CHANGE TO swatch both follow the colour set.
		const int count   = PyramidComponent::ColorSetCount;
		const int current = m_pPyramid->getColorSet();
		m_pPyramid->setColorSet((current + 1 + std::rand() % (count - 1)) % count);

		m_pPyramid->Reset();
		for (auto* qbert : m_qberts)
			qbert->Respawn();
	}
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