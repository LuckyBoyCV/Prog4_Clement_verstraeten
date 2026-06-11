#include "SlickSamComponent.h"
#include "SlickSamMovingState.h"
#include "SlickSamRespawningState.h"
#include "PyramidComponent.h"
#include "GameObject.h"
#include "QbertComponent.h"
#include "GameEvent.h"
#include <cstdlib>

qbert::SlickSamComponent::SlickSamComponent(dae::GameObject* pOwner, PyramidComponent* pyramid, QbertComponent* qbert)
	: Component(pOwner)
	, m_pPyramid(pyramid)
	, m_pQbert(qbert)
{
	// Starts inactive; the enemySpawnerComponent activates Slick/Sam from round 2.
	deactivate();
}

void qbert::SlickSamComponent::activate()
{
	// Enter at a random tile flanking the apex (grid 1 or grid 2)
	m_row = 1;
	m_col = rand() % 2;
	m_futureRow = m_row;
	m_futureCol = m_col;
	m_pendingRespawn = false;
	glm::vec2 pos = m_pPyramid->getTileScreenPos(m_row, m_col);
	m_Owner->SetPosition(pos.x + m_spriteOffsetX, pos.y - m_spriteOffsetY);

	m_pState = std::make_unique<SlickSamMovingState>();
	m_active = true;
}

void qbert::SlickSamComponent::deactivate()
{
	m_active = false;
	m_Owner->SetPosition(-1000.f, -1000.f);
}

void qbert::SlickSamComponent::Update(float deltaTime)
{
	if (!m_active)
		return;

	// OnNotify sets m_pendingRespawn to avoid destroying the state mid-Update
	if (m_pendingRespawn)
	{
		m_pendingRespawn = false;
		SetState(std::make_unique<SlickSamRespawningState>(m_respawnDelay, m_respawnRow, m_respawnCol));
		return;
	}

	if (m_pState)
	{
		auto next = m_pState->Update(*this, deltaTime);
		if (next)
			SetState(std::move(next));
	}
}

bool qbert::SlickSamComponent::isJumping() const
{
	return m_pState && m_pState->isJumping();
}

bool qbert::SlickSamComponent::onTile()
{
	if (auto* tile = m_pPyramid->getTile(m_row, m_col))
		m_pPyramid->reverseStep(*tile);

	if (m_pQbert->getRow() == m_row && m_pQbert->getCol() == m_col)
	{
		m_subject.Notify(dae::GameEvent::EnemyDied, m_Owner);
		m_Owner->SetPosition(-1000.f, -1000.f);
		return true; // caller should transition to respawn
	}
	return false;
}

void qbert::SlickSamComponent::OnNotify(dae::GameEvent event, dae::GameObject*)
{
	if (event != dae::GameEvent::PlayerDied) return;
	if (!m_active) return;

	// Re-enter at a random tile flanking the apex (grid 1 or grid 2)
	m_respawnRow   = 1;
	m_respawnCol   = rand() % 2;
	m_respawnDelay = 1.0f + (rand() % 9) * 0.5f;

	// Hide immediately; state transition applied at start of next Update
	m_Owner->SetPosition(-1000.f, -1000.f);
	m_pendingRespawn = true;
}

void qbert::SlickSamComponent::SetState(std::unique_ptr<SlickSamState> newState)
{
	if (m_pState)
		m_pState->onExit(*this);
	m_pState = std::move(newState);
	if (m_pState)
		m_pState->onEnter(*this);
}
