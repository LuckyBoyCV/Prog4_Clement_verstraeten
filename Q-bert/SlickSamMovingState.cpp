#include "SlickSamMovingState.h"
#include "SlickSamJumpingState.h"
#include "SlickSamRespawningState.h"
#include "SlickSamComponent.h"
#include "QbertComponent.h"
#include "PyramidComponent.h"
#include "GameObject.h"
#include "GameEvent.h"
#include <cstdlib>

std::unique_ptr<qbert::SlickSamState> qbert::SlickSamMovingState::Update(SlickSamComponent& slickSam, float deltaTime)
{
	// Edge-detect Q-bert landing on our tile (Slick/Sam dies, Q-bert is unharmed)
	bool qbertJustLanded = m_qbertWasJumping && !slickSam.getQbert()->isJumping();
	m_qbertWasJumping = slickSam.getQbert()->isJumping();
	if (qbertJustLanded
		&& slickSam.getQbert()->getRow() == slickSam.getRow()
		&& slickSam.getQbert()->getCol() == slickSam.getCol())
	{
		slickSam.m_subject.Notify(dae::GameEvent::EnemyDied, slickSam.getOwner());
		slickSam.getOwner()->SetPosition(-1000.f, -1000.f);
		return std::make_unique<SlickSamRespawningState>(slickSam.getRespawnDuration(), 1, std::rand() % 2);
	}

	m_jumpTime += deltaTime;
	if (m_jumpTime < slickSam.getJumpInterval())
		return nullptr;

	m_jumpTime = 0.f;

	int destRow = slickSam.getRow() + 1;
	int destCol = slickSam.getCol() + (std::rand() % 2);

	if (slickSam.getPyramid()->GetTile(destRow, destCol) != nullptr)
	{
		slickSam.setFutureRow(destRow);
		slickSam.setFutureCol(destCol);
		return std::make_unique<SlickSamJumpingState>();
	}

	// Reached the bottom — re-enter at a random tile flanking the apex (grid 1 or grid 2)
	const int row = 1;
	const int col = std::rand() % 2;
	slickSam.setRow(row);
	slickSam.setCol(col);
	glm::vec2 pos = slickSam.getPyramid()->GetTileScreenPos(row, col);
	slickSam.getOwner()->SetPosition(pos.x + slickSam.getSpriteOffsetX(), pos.y - slickSam.getSpriteOffsetY());
	return std::make_unique<SlickSamMovingState>();
}
