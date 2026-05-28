#include "QbertFallingState.h"
#include "QbertComponent.h"
#include "QbertIdleState.h"
#include "GameObject.h"

qbert::QbertFallingState::QbertFallingState(glm::vec2 fallStartPos)
	: m_fallStartPos(fallStartPos)
{}

std::unique_ptr<qbert::QbertState> qbert::QbertFallingState::Update(QbertComponent& qbert, float deltaTime)
{
	m_timer += deltaTime;
	float t = m_timer / qbert.getRespawnDuration();
	qbert.getOwner()->SetPosition(m_fallStartPos.x, m_fallStartPos.y + t * 600.f);

	if (m_timer >= qbert.getRespawnDuration())
	{
		if (qbert.getLives() > 0)
			qbert.Respawn();
		return std::make_unique<QbertIdleState>();
	}

	return nullptr;
}
