#include "CoilyComponent.h"
#include "PyramidComponent.h"
#include "QbertComponent.h"
#include "CoilyState.h"
#include "CoilyEggState.h"
#include "CoilySnakeState.h"
#include "GameObject.h"


qbert::CoilyComponent::CoilyComponent(dae::GameObject* pOwner, PyramidComponent* pyramid, QbertComponent* qbert)
	: Component(pOwner)
	, m_pPyramid(pyramid)
	, m_pQbert(qbert)
{
	if (auto* tile = m_pPyramid->GetTile(m_row, m_col))
	{
		m_Owner->SetPosition(tile->position.x + 30.f, tile->position.y - 30.f);
	}

	m_pState = std::make_unique<CoilyEggState>();
	m_pState->onEnter(*this);
}

void qbert::CoilyComponent::Update(float deltaTime)
{
	if (m_isJumping)
	{
		m_jumpTime += deltaTime;

		float t = std::min(m_jumpTime / JumpDuration, 1.f);

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

	if (m_pState)
	{
		CoilyState* next = m_pState->Update(*this, deltaTime);
		if (next) 
			SetState(next);
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

void qbert::CoilyComponent::SetSprite(int x, int y, int w, int h)
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

void qbert::CoilyComponent::SetState(CoilyState* newState)
{
	if (m_pState)
		m_pState->onExit(*this);
	m_pState.reset(std::move(newState));
	
	if (m_pState)
		m_pState->onEnter(*this);
	
}
