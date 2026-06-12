#include "QbertComponent.h"
#include "PyramidComponent.h"
#include "Component.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "QbertState.h"
#include "QbertIdleState.h"
#include "QbertJumpingState.h"
#include "QbertFallingState.h"

qbert::QbertComponent::QbertComponent(dae::GameObject* pOwner, PyramidComponent* pyramid, int startRow, int startCol)
	: Component(pOwner),
	m_pPyramid(pyramid),
	m_Lives(3),
	m_row(startRow),
	m_col(startCol),
	m_startRow(startRow),
	m_startCol(startCol)
{
	// Position qbert at the starting tile on the pyramid
	if (auto* tile = m_pPyramid->getTile(m_row, m_col))
		m_Owner->SetPosition(tile->position.x + 30.f, tile->position.y - 30.f);

	// Start in idle state
	m_pState = std::make_unique<QbertIdleState>();
}

void qbert::QbertComponent::Update(float deltaTime)
{
	// Update the current state and transition if necessary
	if (m_pState)
	{
		auto next = m_pState->Update(*this, deltaTime);
		if (next)
			SetState(std::move(next));
	}
}

bool qbert::QbertComponent::Move(int deltaRow, int deltaCol)
{
	// Can't move while jumping, falling, or riding a disc
	if (isJumping() || isFalling() || isRidingDisk())
		return false;

	// Store the target position and start jumping
	m_futureRow = m_row + deltaRow;
	m_futureCol = m_col + deltaCol;
	SetState(std::make_unique<QbertJumpingState>());
	m_subject.Notify(dae::GameEvent::PlayerJumped, m_Owner);
	return true;
}

bool qbert::QbertComponent::isJumping() const
{
	return m_pState && m_pState->isJumping();
}

bool qbert::QbertComponent::isFalling() const
{
	return m_pState && m_pState->isFalling();
}

bool qbert::QbertComponent::isRidingDisk() const
{
	return m_pState && m_pState->isRidingDisk();
}

void qbert::QbertComponent::loseLife()
{
	// Decrement lives and check for game over
	if (m_Lives > 0)
		--m_Lives;

	if (m_Lives == 0)
		m_subject.Notify(dae::GameEvent::GameOver, m_Owner);
	else
		m_subject.Notify(dae::GameEvent::PlayerDied, m_Owner);
}

void qbert::QbertComponent::kill()
{
	// Only kill if not already in a dangerous state
	if (isJumping() || isFalling() || isRidingDisk()) return;

	// Transition to falling state and lose a life
	glm::vec2 pos = m_pPyramid->getTileScreenPos(m_row, m_col);
	SetState(std::make_unique<QbertFallingState>(glm::vec2{ pos.x + 30.f, pos.y - 30.f }));
	loseLife();
}

void qbert::QbertComponent::Respawn()
{
	// Reset to starting position after death
	m_row = m_startRow;
	m_col = m_startCol;
	glm::vec2 pos = m_pPyramid->getTileScreenPos(m_row, m_col);
	m_Owner->SetPosition(pos.x + 30.f, pos.y - 30.f);
}

void qbert::QbertComponent::CancelRide()
{
	// Drop qbert from disc if round refresh happens mid-ride
	if (isRidingDisk())
		SetState(std::make_unique<QbertIdleState>());
}

void qbert::QbertComponent::addScore(int points)
{
	// Award points and notify observers
	m_Score += points;
	m_subject.Notify(dae::GameEvent::PlayerPickedUp, m_Owner);
}

void qbert::QbertComponent::SetState(std::unique_ptr<QbertState> newState)
{
	// Transition states with proper cleanup and initialization
	if (m_pState)
		m_pState->onExit(*this);
	m_pState = std::move(newState);
	if (m_pState)
		m_pState->onEnter(*this);
}
