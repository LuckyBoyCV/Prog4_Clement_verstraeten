#include "CoilyComponent.h"
#include "PyramidComponent.h"
#include "QbertComponent.h"
#include "CoilyState.h"
#include "CoilyEggState.h"
#include "CoilySnakeState.h"
#include "GameObject.h"
#include "GameEvent.h"
#include <cstdlib>


qbert::CoilyComponent::CoilyComponent(dae::GameObject* pOwner, PyramidComponent* pyramid, QbertComponent* qbert)
	: Component(pOwner)
	, m_pPyramid(pyramid)
	, m_pQbert(qbert)
{
	if (auto* tile = m_pPyramid->GetTile(m_row, m_col))
	{
		m_Owner->SetPosition(tile->position.x + 30.f, tile->position.y + m_spriteOffsetY);
	}

	m_pState = std::make_unique<CoilyEggState>();
	m_pState->onEnter(*this);
}

void qbert::CoilyComponent::Update(float deltaTime)
{
	if (m_waitingToRespawn)
	{
		m_respawnTimer += deltaTime;
		if (m_respawnTimer >= m_respawnDelay)
		{
			m_waitingToRespawn = false;
			m_respawnTimer = 0.f;
			m_row = m_respawnRow;
			m_col = m_respawnCol;
			m_isJumping = false;
			m_jumpTime = 0.f;
			SetState(std::make_unique<CoilyEggState>());
		}
		return;
	}

	if (m_isJumping)
	{
		m_jumpTime += deltaTime;

		float t = std::min(m_jumpTime / JumpDuration, 1.f);

		glm::vec2 startPos = m_pPyramid->GetTileScreenPos(m_row, m_col);
		glm::vec2 endPos = m_pPyramid->GetTileScreenPos(m_futureRow, m_futureCol);
		float arc = -4.f * t * (t - 1.f);

		m_Owner->SetPosition(
			startPos.x + (endPos.x - startPos.x) * t + 30.f,
			startPos.y + (endPos.y - startPos.y) * t - arc * 30.f + m_spriteOffsetY
		);

		if (t >= 1.f)
		{
			m_isJumping = false;
			m_row = m_futureRow;
			m_col = m_futureCol;
			m_Owner->SetPosition(endPos.x + 30.f, endPos.y + m_spriteOffsetY);
			onTile();
		}
		return;
	}

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
	if(m_isJumping)
	{
		return false;
	}
	m_futureRow = destRow;
	m_futureCol = destCol;
	m_isJumping = true;
	m_jumpTime = 0.f;
	return true;
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

	static const int rows[] = { 0, 1, 1 };
	static const int cols[] = { 0, 0, 1 };
	int idx = rand() % 3;
	m_respawnRow = rows[idx];
	m_respawnCol = cols[idx];
	m_respawnDelay = 1.0f + (rand() % 9) * 0.5f;
	m_respawnTimer = 0.f;
	m_isSnake = false;
	m_isJumping = false;
	m_waitingToRespawn = true;
	m_Owner->SetPosition(-1000.f, -1000.f);
}

void qbert::CoilyComponent::onTile()
{
	if (!m_isSnake) return;
	if (m_pQbert->getRow() == m_row && m_pQbert->getCol() == m_col)
		m_pQbert->kill();
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
	if (auto* tile = m_pPyramid->GetTile(m_row, m_col))
	{
		m_Owner->SetPosition(tile->position.x + 30.f, tile->position.y + m_spriteOffsetY);
	}
}
