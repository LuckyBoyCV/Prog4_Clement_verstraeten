#include "QbertComponent.h"
#include "PyramidComponent.h"
#include "Component.h"
#include "GameObject.h"
#include "SceneManager.h"
#include <algorithm>

qbert::QbertComponent::QbertComponent(dae::GameObject* pOwner, PyramidComponent* pyramid, int startRow, int startCol)
	: Component(pOwner),
	m_pPyramid(pyramid),
	m_Lives(3),
	m_row(startRow),
	m_col(startCol)
{
	if (auto* tile = m_pPyramid->GetTile(m_row, m_col))
	{
		
		m_Owner->SetPosition(tile->position.x +30.f, tile->position.y - 30.f);
	}
}

void qbert::QbertComponent::Update(float deltaTime)
{
	if(m_isFalling)
	{
		m_respawnTimer += deltaTime;
		float t = m_respawnTimer / m_respawnDuration;
		m_Owner->SetPosition(m_fallStartPos.x, m_fallStartPos.y + t * 600.f);
		if (m_respawnTimer >= m_respawnDuration)
		{
			m_isFalling = false;
			m_respawnTimer = 0.f;
			if (m_Lives > 0)
			{
				Respawn();
			}
		}
		return;
	}
		
	if (!m_isJumping)	
		return;

	m_jumpTime += deltaTime;
	float t = std::min(m_jumpTime / m_jumpDuration, 1.f);  //stays between 0 and 1  and midway is 0.5

	glm::vec2 startPos = m_pPyramid->GetTileScreenPos(m_row, m_col);
	glm::vec2 endPos = m_pPyramid->GetTileScreenPos(m_futureRow, m_futureCol);

	float xMove = startPos.x + (endPos.x - startPos.x) * t + 30.f; // interpolate
	float Arc = -4.f * t * (t - 1.f);
	float yMove = startPos.y + (endPos.y - startPos.y) * t - Arc * 30.f - 30.f; //interpolate and arc height

	m_Owner->SetPosition(xMove, yMove);

	if (t >= 1.f)
	{
		m_isJumping = false;


		if (!m_pPyramid->GetTile(m_futureRow, m_futureCol))
		{
			m_isFalling = true;
			m_fallStartPos = { endPos.x + 30.f, endPos.y - 30.f };
			loseLife();
			return;
		}

		m_row = m_futureRow;
		m_col = m_futureCol;
		m_pPyramid->StepOn(*m_pPyramid->GetTile(m_row, m_col));
		m_Owner->SetPosition(endPos.x + 30.f, endPos.y - 30.f);
	}
}

bool qbert::QbertComponent::Move(int destRow, int destCol)
{
	if (m_isJumping || m_isFalling)
		return false;

	m_futureRow = m_row + destRow;
	m_futureCol = m_col + destCol;

	m_jumpTime = 0.f;
	m_isJumping = true;
		
	return true;
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

void qbert::QbertComponent::Respawn()
{

	m_row = 0;
	m_col = 0;
	m_Owner->SetPosition(m_pPyramid->GetTileScreenPos(m_row, m_col).x + 30.f, m_pPyramid->GetTileScreenPos(m_row, m_col).y - 30.f);
}


