#include "QbertComponent.h"
#include "PyramidComponent.h"
#include "Component.h"
#include "GameObject.h"
#include "Event.h"
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
		m_pPyramid->StepOn(*tile);
		m_Owner->SetPosition(tile->position.x, tile->position.y);
	}
}

//void qbert::QbertComponent::loseLife()
//{
//	if (m_Lives > 0)
//		--m_Lives;
//
//	if (m_Lives == 0)
//		m_subject.Notify(dae::GameEvent::GameOver, m_Owner);
//	else
//		m_subject.Notify(dae::GameEvent::PlayerDied, m_Owner);
//}
