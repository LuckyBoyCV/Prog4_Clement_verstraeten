#include "CoilyRespawningState.h"
#include "CoilyEggState.h"
#include "CoilyComponent.h"

qbert::CoilyRespawningState::CoilyRespawningState(float delay, int respawnRow, int respawnCol)
	: m_delay(delay)
	, m_respawnRow(respawnRow)
	, m_respawnCol(respawnCol)
{}

std::unique_ptr<qbert::CoilyState> qbert::CoilyRespawningState::Update(CoilyComponent& coily, float deltaTime)
{
	m_timer += deltaTime;
	if (m_timer < m_delay)
		return nullptr;

	// Delay done — place coily at the spawn tile and let him hatch
	coily.setRow(m_respawnRow);
	coily.setCol(m_respawnCol);
	return std::make_unique<CoilyEggState>();
}
