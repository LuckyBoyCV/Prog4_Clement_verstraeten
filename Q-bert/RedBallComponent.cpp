#include "RedBallComponent.h"
#include "RedBallMovingState.h"
#include "RedBallRespawningState.h"
#include "PyramidComponent.h"
#include "QbertComponent.h"
#include "GameObject.h"
#include "GameEvent.h"
#include <cstdlib>

qbert::redBallComponent::redBallComponent(dae::GameObject* pOwner, PyramidComponent* pyramid, QbertComponent* qbert)
	: Component(pOwner)
	, m_pPyramid(pyramid)
	, m_pQbert(qbert)
{
	// Starts inactive; the enemySpawnerComponent activates the red ball from round 3.
	deactivate();
}

void qbert::redBallComponent::activate()
{
	// Enter at a random tile flanking the apex (grid 1 or grid 2)
	m_row = 1;
	m_col = rand() % 2;
	m_pendingRespawn = false;
	setSprite(17, 16, 16, 16);
	glm::vec2 pos = m_pPyramid->getTileScreenPos(m_row, m_col);
	m_Owner->SetPosition(pos.x + m_spriteOffsetX, pos.y - m_spriteOffsetY);

	m_pState = std::make_unique<RedBallMovingState>();
	m_active = true;
}

void qbert::redBallComponent::deactivate()
{
	m_active = false;
	m_Owner->SetPosition(-1000.f, -1000.f);
}

void qbert::redBallComponent::Update(float deltaTime)
{
	if (!m_active)
		return;

	// OnNotify sets m_pendingRespawn to avoid destroying the state mid-Update
	if (m_pendingRespawn)
	{
		m_pendingRespawn = false;
		SetState(std::make_unique<RedBallRespawningState>(m_respawnDelay, m_respawnRow, m_respawnCol));
		return;
	}

	if (m_pState)
	{
		auto next = m_pState->Update(*this, deltaTime);
		if (next)
			SetState(std::move(next));
	}
}

bool qbert::redBallComponent::isJumping() const
{
	return m_pState && m_pState->isJumping();
}

void qbert::redBallComponent::setSprite(int x, int y, int w, int h)
{
	if (auto* render = m_Owner->GetComponent<dae::RenderComponent>())
		render->SetSourceRect(x, y, w, h);
}

void qbert::redBallComponent::OnNotify(dae::GameEvent event, dae::GameObject*)
{
	if (event != dae::GameEvent::PlayerDied) return;
	if (!m_active) return;

	// Re-enter at a random tile flanking the apex (grid 1 or grid 2)
	m_respawnRow   = 1;
	m_respawnCol   = rand() % 2;
	m_respawnDelay = 1.0f + (rand() % 9) * 0.5f;

	// Hide ball immediately; state transition applied at start of next Update
	m_Owner->SetPosition(-1000.f, -1000.f);
	m_pendingRespawn = true;
}

void qbert::redBallComponent::onTile()
{
	if (m_pQbert->getRow() == m_row && m_pQbert->getCol() == m_col)
		m_pQbert->kill();
}

void qbert::redBallComponent::SetState(std::unique_ptr<RedBallState> newState)
{
	if (m_pState)
		m_pState->onExit(*this);
	m_pState = std::move(newState);
	if (m_pState)
		m_pState->onEnter(*this);
}
