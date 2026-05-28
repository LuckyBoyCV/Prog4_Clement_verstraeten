#include "RedBallMovingState.h"
#include "RedBallJumpingState.h"
#include "RedBallComponent.h"
#include "QbertComponent.h"
#include "PyramidComponent.h"
#include "GameObject.h"
#include <cstdlib>

std::unique_ptr<qbert::RedBallState> qbert::RedBallMovingState::Update(redBallComponent& ball, float deltaTime)
{
	// Edge-detect Q-bert landing on our tile
	bool qbertJustLanded = m_qbertWasJumping && !ball.getQbert()->isJumping();
	m_qbertWasJumping = ball.getQbert()->isJumping();
	if (qbertJustLanded && ball.getQbert()->getRow() == ball.getRow() && ball.getQbert()->getCol() == ball.getCol())
		ball.getQbert()->kill();

	m_jumpTime += deltaTime;
	if (m_jumpTime < ball.getJumpInterval())
		return nullptr;

	m_jumpTime = 0.f;

	int destRow = ball.getRow() + 1;
	int destCol = ball.getCol() + (std::rand() % 2);

	if (ball.getPyramid()->GetTile(destRow, destCol) != nullptr)
	{
		ball.setFutureRow(destRow);
		ball.setFutureCol(destCol);
		return std::make_unique<RedBallJumpingState>();
	}

	// No tile to land on — reset to the top of the pyramid
	ball.setRow(0);
	ball.setCol(0);
	ball.setSprite(17, 16, 16, 16);
	glm::vec2 pos = ball.getPyramid()->GetTileScreenPos(0, 0);
	ball.getOwner()->SetPosition(pos.x + ball.getSpriteOffsetX(), pos.y - ball.getSpriteOffsetY());
	return std::make_unique<RedBallMovingState>();
}
