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
	if (auto* tile = m_pPyramid->GetTile(m_row, m_col))
		m_Owner->SetPosition(tile->position.x + 30.f, tile->position.y - 30.f);

	m_pState = std::make_unique<QbertIdleState>();
}

void qbert::QbertComponent::Update(float deltaTime)
{
	if (m_pState)
	{
		auto next = m_pState->Update(*this, deltaTime);
		if (next)
			SetState(std::move(next));
	}
}

bool qbert::QbertComponent::Move(int deltaRow, int deltaCol)
{
	if (isJumping() || isFalling())
		return false;

	m_futureRow = m_row + deltaRow;
	m_futureCol = m_col + deltaCol;
	SetState(std::make_unique<QbertJumpingState>());
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

void qbert::QbertComponent::loseLife()
{
	if (m_Lives > 0)
		--m_Lives;

	if (m_Lives == 0)
		m_subject.Notify(dae::GameEvent::GameOver, m_Owner);
	else
		m_subject.Notify(dae::GameEvent::PlayerDied, m_Owner);
}

void qbert::QbertComponent::kill()
{
	if (isJumping() || isFalling()) return;

	glm::vec2 pos = m_pPyramid->GetTileScreenPos(m_row, m_col);
	SetState(std::make_unique<QbertFallingState>(glm::vec2{ pos.x + 30.f, pos.y - 30.f }));
	loseLife();
}

void qbert::QbertComponent::Respawn()
{
	m_row = m_startRow;
	m_col = m_startCol;
	glm::vec2 pos = m_pPyramid->GetTileScreenPos(m_row, m_col);
	m_Owner->SetPosition(pos.x + 30.f, pos.y - 30.f);
}

void qbert::QbertComponent::SetState(std::unique_ptr<QbertState> newState)
{
	if (m_pState)
		m_pState->onExit(*this);
	m_pState = std::move(newState);
	if (m_pState)
		m_pState->onEnter(*this);
}
