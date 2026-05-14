#include "RedBallComponent.h"
#include "PyramidComponent.h"
#include "GameObject.h"

qbert::redBallComponent::redBallComponent(dae::GameObject* pOwner, PyramidComponent* pyramid, QbertComponent* qbert)
	: Component(pOwner)
	, m_pPyramid(pyramid)
	, m_pQbert(qbert)
{
	if (auto* tile = m_pPyramid->GetTile(m_row, m_col))
	{
		m_Owner->SetPosition(tile->position.x + 30.f, tile->position.y - 30.f);
	}
}

void qbert::redBallComponent::Update(float deltaTime)
{

	if(m_isJumping)
	{
		m_jumpTime += deltaTime;
		float t = std::min(m_jumpTime / m_jumpDuration, 1.f);
		glm::vec2 startPos = m_pPyramid->GetTileScreenPos(m_row, m_col);
		glm::vec2 endPos = m_pPyramid->GetTileScreenPos(m_futureRow, m_futureCol);
		float arc = -4.f * t * (t - 1.f);
		m_Owner->SetPosition(
			startPos.x + (endPos.x - startPos.x) * t + 30.f,
			startPos.y + (endPos.y - startPos.y) * t - arc * 30.f - 30.f
		);
		if (t >= 1.f)
		{
			m_isJumping = false;
			m_row = m_futureRow;
			m_col = m_futureCol;
			m_Owner->SetPosition(endPos.x + 30.f, endPos.y - 30.f);
		}
		return;


	}

	m_jumpTime += deltaTime;
	if(m_jumpTime < m_jumpInterval)
	{
		return;
	}

	m_jumpTime = 0.f;

	int destRow = m_row + 1;
	int destCol = m_col + (std::rand() % 2);

	if (!Move(destRow, destCol))
	{
		
		Respawn();
	}
}

bool qbert::redBallComponent::Move(int destRow, int destCol)
{
	if(m_isJumping)
		return false;

	if (m_pPyramid->GetTile(destRow, destCol) == nullptr)
		return false;
	m_futureRow = destRow;
	m_futureCol = destCol;
	m_isJumping = true;
	return true;
}

void qbert::redBallComponent::setSprite(int x, int y, int w, int h)
{
	if (auto* render = m_Owner->GetComponent<dae::RenderComponent>())
	{
		render->SetSourceRect(x, y, w, h);
	}
}

void qbert::redBallComponent::Respawn()
{
	m_row = 0;
	m_col = 0;
	m_Owner->SetPosition(m_pPyramid->GetTileScreenPos(m_row, m_col).x + 30.f, m_pPyramid->GetTileScreenPos(m_row, m_col).y - 30.f);
}
