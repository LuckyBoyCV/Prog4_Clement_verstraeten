#include "CoilyComponent.h"
#include "PyramidComponent.h"
#include "QbertComponent.h"
#include "CoilyState.h"
#include "CoilyEggState.h"
#include "CoilyJumpingState.h"
#include "CoilyPlayerState.h"
#include "CoilyRespawningState.h"
#include "GameObject.h"
#include "GameEvent.h"
#include <cstdlib>

qbert::CoilyComponent::CoilyComponent(dae::GameObject* pOwner, PyramidComponent* pyramid, QbertComponent* qbert)
	: Component(pOwner)
	, m_pPyramid(pyramid)
	, m_pQbert(qbert)
{
	// Starts inactive; the enemySpawnerComponent activates Coily each round.
	deactivate();
}

void qbert::CoilyComponent::activate()
{
	// Enter at a random tile flanking the apex (grid 1 or grid 2), never the apex itself
	m_row = 1;
	m_col = rand() % 2;
	m_isSnake = false;
	m_qbertWasJumping = false;
	m_pendingRespawn = false;
	m_pendingJump = false;

	m_pState = std::make_unique<CoilyEggState>();
	m_pState->onEnter(*this);

	if (auto* tile = m_pPyramid->getTile(m_row, m_col))
		m_Owner->SetPosition(tile->position.x + 30.f, tile->position.y + m_spriteOffsetY);

	m_active = true;
}

void qbert::CoilyComponent::deactivate()
{
	m_active = false;
	m_Owner->SetPosition(-1000.f, -1000.f);
}

void qbert::CoilyComponent::Update(float deltaTime)
{
	if (!m_active)
		return;

	// avoids mid update state change
	if (m_pendingRespawn)
	{
		m_pendingRespawn = false;
		SetState(std::make_unique<CoilyRespawningState>(m_respawnDelay, m_respawnRow, m_respawnCol));
		return;
	}

	// same here but player coily
	if (m_pendingJump)
	{
		m_pendingJump = false;
		SetState(std::make_unique<CoilyJumpingState>(m_pendingJumpRow, m_pendingJumpCol, std::make_unique<coilyPlayerState>()));
		return;
	}

	// Kill Q*bert if he lands on this tile while Coily is a snake
	bool qbertJustLanded = m_qbertWasJumping && !m_pQbert->isJumping();
	m_qbertWasJumping = m_pQbert->isJumping();
	if (m_isSnake && qbertJustLanded && m_pQbert->getRow() == m_row && m_pQbert->getCol() == m_col)
		m_pQbert->kill();

	if (m_pState)
	{
		auto next = m_pState->Update(*this, deltaTime);
		if (next)
			SetState(std::move(next));
	}
}

bool qbert::CoilyComponent::Move(int destRow, int destCol)
{
	if (isJumping())
		return false;
	m_pendingJumpRow = destRow;
	m_pendingJumpCol = destCol;
	m_pendingJump = true;
	return true;
}

bool qbert::CoilyComponent::isJumping() const
{
	return m_pState && m_pState->isJumping();
}

void qbert::CoilyComponent::setSprite(int x, int y, int w, int h)
{
	if (auto* render = m_Owner->GetComponent<dae::RenderComponent>())
		render->SetSourceRect(x, y, w, h);
}

int qbert::CoilyComponent::getQbertRow() const
{
	return m_pQbert->getRow();
}

int qbert::CoilyComponent::getQbertCol() const
{
	return m_pQbert->getCol();
}

void qbert::CoilyComponent::OnNotify(dae::GameEvent event, dae::GameObject*)
{
	if (event != dae::GameEvent::PlayerDied) return;
	if (!m_active) return;

	// Re-enter at a random tile flanking the apex (grid 1 or grid 2)
	m_respawnRow   = 1;
	m_respawnCol   = rand() % 2;
	m_respawnDelay = 1.0f + (rand() % 9) * 0.5f;
	m_isSnake = false;
	m_Owner->SetPosition(-1000.f, -1000.f);
	m_pendingRespawn = true;
}

void qbert::CoilyComponent::onTile()
{
	if (!m_isSnake) return;
	if (m_pQbert->getRow() == m_row && m_pQbert->getCol() == m_col)
		m_pQbert->kill();
}

void qbert::CoilyComponent::die()
{
	if (!m_active) return;

	// Bonus for luring Coily off the edge. Coily has no Subject of its own, so the score
	// goes through Q*bert (which also notifies the score display).
	if (m_pQbert)
		m_pQbert->addScore(500);

	// Re-enter as a fresh egg after a short delay, same as the post-PlayerDied respawn.
	m_respawnRow   = 1;
	m_respawnCol   = rand() % 2;
	m_respawnDelay = 1.0f + (rand() % 9) * 0.5f;
	m_isSnake = false;
	m_Owner->SetPosition(-1000.f, -1000.f);
	m_pendingRespawn = true;
}

void qbert::CoilyComponent::SetState(std::unique_ptr<CoilyState> newState)
{
	if (m_pState)
		m_pState->onExit(*this);
	m_pState = std::move(newState);
	if (m_pState)
		m_pState->onEnter(*this);
	SnapToCurrentTile();
}

void qbert::CoilyComponent::SnapToCurrentTile()
{
	if (auto* tile = m_pPyramid->getTile(m_row, m_col))
		m_Owner->SetPosition(tile->position.x + 30.f, tile->position.y + m_spriteOffsetY);
}
