#include "UggWrongwayMovingState.h"
#include "UggWrongwayJumpingState.h"
#include "UggWrongwayComponent.h"
#include "QbertComponent.h"
#include "PyramidComponent.h"
#include "GameObject.h"

std::unique_ptr<qbert::UggWrongwayState> qbert::UggWrongwayMovingState::Update(UggWrongwayComponent& uggWrongway, float deltaTime)
{
	// Edge detect qbert landing on our tile, contact costs him a life and we stay unharmed
	bool qbertJustLanded = m_qbertWasJumping && !uggWrongway.getQbert()->isJumping();
	m_qbertWasJumping = uggWrongway.getQbert()->isJumping();
	if (qbertJustLanded
		&& uggWrongway.getQbert()->getRow() == uggWrongway.getRow()
		&& uggWrongway.getQbert()->getCol() == uggWrongway.getCol())
	{
		uggWrongway.getQbert()->kill();
		return nullptr; // PlayerDied, pending respawn is handled at the next Update
	}

	m_jumpTime += deltaTime;
	if (m_jumpTime < uggWrongway.getJumpInterval())
		return nullptr;

	m_jumpTime = 0.f;

	// Climb one tile up this enemy's edge, up and inward toward the apex
	int destRow = uggWrongway.getRow() - 1;
	int destCol = uggWrongway.getCol() - uggWrongway.getColStep();

	if (uggWrongway.getPyramid()->getTile(destRow, destCol) != nullptr)
	{
		uggWrongway.setFutureRow(destRow);
		uggWrongway.setFutureCol(destCol);
		return std::make_unique<UggWrongwayJumpingState>();
	}

	// Reached the apex, respawn at the bottom corner of this enemy's edge
	const int row = uggWrongway.getSpawnRow();
	const int col = uggWrongway.getSpawnCol();
	uggWrongway.setRow(row);
	uggWrongway.setCol(col);
	glm::vec2 pos = uggWrongway.getPyramid()->getTileScreenPos(row, col);
	uggWrongway.getOwner()->SetPosition(pos.x + uggWrongway.getSpriteOffsetX(), pos.y - uggWrongway.getSpriteOffsetY());
	return std::make_unique<UggWrongwayMovingState>();
}
