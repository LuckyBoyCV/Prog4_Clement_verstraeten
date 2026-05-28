#include "RedBallJumpingState.h"
#include "RedBallMovingState.h"
#include "RedBallComponent.h"
#include "PyramidComponent.h"
#include "GameObject.h"
#include <algorithm>

std::unique_ptr<qbert::RedBallState> qbert::RedBallJumpingState::Update(redBallComponent& ball, float deltaTime)
{
	m_jumpTime += deltaTime;
	float t = std::min(m_jumpTime / ball.getJumpDuration(), 1.f);

	glm::vec2 startPos = ball.getPyramid()->GetTileScreenPos(ball.getRow(), ball.getCol());
	glm::vec2 endPos   = ball.getPyramid()->GetTileScreenPos(ball.getFutureRow(), ball.getFutureCol());

	float arc = -4.f * t * (t - 1.f);
	ball.getOwner()->SetPosition(
		startPos.x + (endPos.x - startPos.x) * t + ball.getSpriteOffsetX(),
		startPos.y + (endPos.y - startPos.y) * t - arc * 30.f - ball.getSpriteOffsetY()
	);

	if (t >= 1.f)
	{
		ball.setRow(ball.getFutureRow());
		ball.setCol(ball.getFutureCol());
		ball.getOwner()->SetPosition(endPos.x + ball.getSpriteOffsetX(), endPos.y - ball.getSpriteOffsetY());
		ball.onTile();
		return std::make_unique<RedBallMovingState>();
	}

	return nullptr;
}
