#include "RedBallRespawningState.h"
#include "RedBallMovingState.h"
#include "RedBallComponent.h"
#include "PyramidComponent.h"
#include "GameObject.h"

qbert::RedBallRespawningState::RedBallRespawningState(float delay, int respawnRow, int respawnCol)
	: m_delay(delay)
	, m_respawnRow(respawnRow)
	, m_respawnCol(respawnCol)
{}

std::unique_ptr<qbert::RedBallState> qbert::RedBallRespawningState::Update(redBallComponent& ball, float deltaTime)
{
	m_timer += deltaTime;
	if (m_timer < m_delay)
		return nullptr;

	// Delay elapsed — reappear at the assigned spawn tile
	ball.setRow(m_respawnRow);
	ball.setCol(m_respawnCol);
	ball.setSprite(17, 16, 16, 16);
	glm::vec2 pos = ball.getPyramid()->GetTileScreenPos(m_respawnRow, m_respawnCol);
	ball.getOwner()->SetPosition(pos.x + ball.getSpriteOffsetX(), pos.y - ball.getSpriteOffsetY());
	return std::make_unique<RedBallMovingState>();
}
