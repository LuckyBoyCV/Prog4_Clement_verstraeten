#include "SlickSamRespawningState.h"
#include "SlickSamMovingState.h"
#include "SlickSamComponent.h"
#include "PyramidComponent.h"
#include "GameObject.h"

qbert::SlickSamRespawningState::SlickSamRespawningState(float delay, int respawnRow, int respawnCol)
	: m_delay(delay)
	, m_respawnRow(respawnRow)
	, m_respawnCol(respawnCol)
{}

std::unique_ptr<qbert::SlickSamState> qbert::SlickSamRespawningState::Update(SlickSamComponent& slickSam, float deltaTime)
{
	m_timer += deltaTime;
	if (m_timer < m_delay)
		return nullptr;

	// Delay elapsed  reappear at the assigned spawn tile
	slickSam.setRow(m_respawnRow);
	slickSam.setCol(m_respawnCol);
	glm::vec2 pos = slickSam.getPyramid()->getTileScreenPos(m_respawnRow, m_respawnCol);
	slickSam.getOwner()->SetPosition(pos.x + slickSam.getSpriteOffsetX(), pos.y - slickSam.getSpriteOffsetY());
	return std::make_unique<SlickSamMovingState>();
}
