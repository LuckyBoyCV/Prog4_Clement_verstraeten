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
	// Start at top of pyramid
	m_row = 0;
	m_col = 0;
	setSprite(17, 16, 16, 16);
	glm::vec2 pos = m_pPyramid->GetTileScreenPos(m_row, m_col);
	m_Owner->SetPosition(pos.x + m_spriteOffsetX, pos.y - m_spriteOffsetY);

	m_pState = std::make_unique<RedBallMovingState>();
}

void qbert::redBallComponent::Update(float deltaTime)
{
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

	static const int rows[] = { 0, 1, 1 };
	static const int cols[] = { 0, 0, 1 };
	int idx = rand() % 3;
	m_respawnRow   = rows[idx];
	m_respawnCol   = cols[idx];
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
