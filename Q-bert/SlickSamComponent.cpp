#include "SlickSamComponent.h"
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
	if (auto* tile = m_pPyramid->GetTile(m_row, m_col))
	{
		m_Owner->SetPosition(tile->position.x + m_spriteOffsetX, tile->position.y - m_spriteOffsetY);
	}
	Respawn();
}

void qbert::SlickSamComponent::Update(float deltaTime)
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
			glm::vec2 pos = m_pPyramid->GetTileScreenPos(m_row, m_col);
			m_Owner->SetPosition(pos.x + m_spriteOffsetX, pos.y - m_spriteOffsetY);
		}
		return;
	}

	if (m_isJumping)
	{
		m_jumpTime += deltaTime;
		float t = std::min(m_jumpTime / m_jumpDuration, 1.f);
		glm::vec2 startPos = m_pPyramid->GetTileScreenPos(m_row, m_col);
		glm::vec2 endPos = m_pPyramid->GetTileScreenPos(m_futureRow, m_futureCol);
		float arc = -4.f * t * (t - 1.f);
		m_Owner->SetPosition(
			startPos.x + (endPos.x - startPos.x) * t + m_spriteOffsetX,
			startPos.y + (endPos.y - startPos.y) * t - arc * 30.f - 15.f
		);
		if (t >= 1.f)
		{
			m_isJumping = false;
			m_row = m_futureRow;
			m_col = m_futureCol;
			m_Owner->SetPosition(endPos.x + m_spriteOffsetX, endPos.y - m_spriteOffsetY);
			onTile();
		}
		return;
	}

	bool qbertJustLanded = m_qbertWasJumping && !m_pQbert->isJumping();
	m_qbertWasJumping = m_pQbert->isJumping();
	if (qbertJustLanded && m_pQbert->getRow() == m_row && m_pQbert->getCol() == m_col)
	{
		m_subject.Notify(dae::GameEvent::EnemyDied, m_Owner);
		m_waitingToRespawn = true;
		m_respawnTimer = 0.f;
		m_respawnDelay = m_respawnDuration;
		m_respawnRow = 0;
		m_respawnCol = 0;
		m_Owner->SetPosition(-1000.f, -1000.f);
		return;
	}

	m_jumpTime += deltaTime;
	if (m_jumpTime < m_jumpInterval)
		return;

	m_jumpTime = 0.f;

	int destRow = m_row + 1;
	int destCol = m_col + (std::rand() % 2);

	if (!Move(destRow, destCol))
		Respawn();
}

bool qbert::SlickSamComponent::Move(int destRow, int destCol)
{
	if (m_isJumping)
		return false;

	if (m_pPyramid->GetTile(destRow, destCol) == nullptr)
		return false;
	m_futureRow = destRow;
	m_futureCol = destCol;
	m_isJumping = true;
	return true;
}


void qbert::SlickSamComponent::onTile()
{
	if (auto* tile = m_pPyramid->GetTile(m_row, m_col))
		m_pPyramid->ReverseStep(*tile);

	if (m_pQbert->getRow() == m_row && m_pQbert->getCol() == m_col)
	{
		m_subject.Notify(dae::GameEvent::EnemyDied, m_Owner);
		m_waitingToRespawn = true;
		m_respawnTimer = 0.f;
		m_respawnDelay = m_respawnDuration;
		m_respawnRow = 0;
		m_respawnCol = 0;
		m_Owner->SetPosition(-1000.f, -1000.f);
	}
}

void qbert::SlickSamComponent::OnNotify(dae::GameEvent event, dae::GameObject*)
{
	if (event != dae::GameEvent::PlayerDied) return;

	static const int rows[] = { 0, 1, 1 };
	static const int cols[] = { 0, 0, 1 };
	int idx = rand() % 3;
	m_respawnRow = rows[idx];
	m_respawnCol = cols[idx];
	m_respawnDelay = 1.0f + (rand() % 9) * 0.5f;
	m_respawnTimer = 0.f;
	m_isJumping = false;
	m_waitingToRespawn = true;
	m_Owner->SetPosition(-1000.f, -1000.f);
}

void qbert::SlickSamComponent::Respawn()
{
	m_row = 0;
	m_col = 0;
	m_Owner->SetPosition(m_pPyramid->GetTileScreenPos(m_row, m_col).x + m_spriteOffsetX, m_pPyramid->GetTileScreenPos(m_row, m_col).y - m_spriteOffsetY);
}
