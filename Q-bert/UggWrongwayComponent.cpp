#include "UggWrongwayComponent.h"
#include "UggWrongwayMovingState.h"
#include "UggWrongwayRespawningState.h"
#include "PyramidComponent.h"
#include "GameObject.h"
#include "QbertComponent.h"
#include "GameEvent.h"
#include <cstdlib>

qbert::UggWrongwayComponent::UggWrongwayComponent(dae::GameObject* pOwner, PyramidComponent* pyramid, QbertComponent* qbert, UggWrongwaySide side)
	: Component(pOwner)
	, m_pPyramid(pyramid)
	, m_pQbert(qbert)
	, m_side(side)
{
	// Starts inactive, the enemySpawnerComponent activates Ugg/Wrongway from round 4
	deactivate();
}

int qbert::UggWrongwayComponent::getSpawnRow() const
{
	// Both enter at the bottom row and climb toward the apex
	return PyramidComponent::Rows - 1;
}

int qbert::UggWrongwayComponent::getSpawnCol() const
{
	// Right edge Ugg starts at the bottom right corner where col stays row, the left edge
	// Wrongway starts at the bottom left corner where col stays 0
	return m_side == UggWrongwaySide::right ? PyramidComponent::Rows - 1 : 0;
}

void qbert::UggWrongwayComponent::activate()
{
	// Enter at the bottom corner of this enemy's edge and climb toward the apex
	m_row = getSpawnRow();
	m_col = getSpawnCol();
	m_futureRow = m_row;
	m_futureCol = m_col;
	m_pendingRespawn = false;
	glm::vec2 pos = m_pPyramid->getTileScreenPos(m_row, m_col);
	m_Owner->SetPosition(pos.x + m_spriteOffsetX, pos.y - m_spriteOffsetY);

	m_pState = std::make_unique<UggWrongwayMovingState>();
	m_active = true;
}

void qbert::UggWrongwayComponent::deactivate()
{
	m_active = false;
	m_Owner->SetPosition(-1000.f, -1000.f);
}

void qbert::UggWrongwayComponent::Update(float deltaTime)
{
	if (!m_active)
		return;

	// OnNotify sets m_pendingRespawn so we don't destroy the state mid Update
	if (m_pendingRespawn)
	{
		m_pendingRespawn = false;
		SetState(std::make_unique<UggWrongwayRespawningState>(m_respawnDelay, m_respawnRow, m_respawnCol));
		return;
	}

	if (m_pState)
	{
		auto next = m_pState->Update(*this, deltaTime);
		if (next)
			SetState(std::move(next));
	}
}

bool qbert::UggWrongwayComponent::isJumping() const
{
	return m_pState && m_pState->isJumping();
}

void qbert::UggWrongwayComponent::onTile()
{
	// Ugg/Wrongway are hazards, they never flip tiles, if Q*bert shares the tile we just
	// landed on he loses a life, kill does nothing while he is airborne or safe
	if (m_pQbert->getRow() == m_row && m_pQbert->getCol() == m_col)
		m_pQbert->kill();
}

void qbert::UggWrongwayComponent::OnNotify(dae::GameEvent event, dae::GameObject*)
{
	if (event != dae::GameEvent::PlayerDied) return;
	if (!m_active) return;

	// Respawn at the bottom corner of this enemy's edge after a short random delay
	m_respawnRow   = getSpawnRow();
	m_respawnCol   = getSpawnCol();
	m_respawnDelay = 1.0f + (rand() % 9) * 0.5f;

	// Hide immediately; state transition applied at start of next Update
	m_Owner->SetPosition(-1000.f, -1000.f);
	m_pendingRespawn = true;
}

void qbert::UggWrongwayComponent::SetState(std::unique_ptr<UggWrongwayState> newState)
{
	if (m_pState)
		m_pState->onExit(*this);
	m_pState = std::move(newState);
	if (m_pState)
		m_pState->onEnter(*this);
}
