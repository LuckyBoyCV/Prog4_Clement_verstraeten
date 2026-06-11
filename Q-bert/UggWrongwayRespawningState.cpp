#include "UggWrongwayRespawningState.h"
#include "UggWrongwayMovingState.h"
#include "UggWrongwayComponent.h"
#include "PyramidComponent.h"
#include "GameObject.h"

qbert::UggWrongwayRespawningState::UggWrongwayRespawningState(float delay, int respawnRow, int respawnCol)
	: m_delay(delay)
	, m_respawnRow(respawnRow)
	, m_respawnCol(respawnCol)
{}

std::unique_ptr<qbert::UggWrongwayState> qbert::UggWrongwayRespawningState::Update(UggWrongwayComponent& uggWrongway, float deltaTime)
{
	m_timer += deltaTime;
	if (m_timer < m_delay)
		return nullptr;

	// Delay elapsed, reappear at the assigned edge bottom corner
	uggWrongway.setRow(m_respawnRow);
	uggWrongway.setCol(m_respawnCol);
	glm::vec2 pos = uggWrongway.getPyramid()->getTileScreenPos(m_respawnRow, m_respawnCol);
	uggWrongway.getOwner()->SetPosition(pos.x + uggWrongway.getSpriteOffsetX(), pos.y - uggWrongway.getSpriteOffsetY());
	return std::make_unique<UggWrongwayMovingState>();
}
