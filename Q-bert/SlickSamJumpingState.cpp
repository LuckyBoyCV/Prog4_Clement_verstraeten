#include "SlickSamJumpingState.h"
#include "SlickSamMovingState.h"
#include "SlickSamRespawningState.h"
#include "SlickSamComponent.h"
#include "PyramidComponent.h"
#include "GameObject.h"
#include <algorithm>

std::unique_ptr<qbert::SlickSamState> qbert::SlickSamJumpingState::Update(SlickSamComponent& slickSam, float deltaTime)
{
	m_jumpTime += deltaTime;
	float t = std::min(m_jumpTime / slickSam.getJumpDuration(), 1.f);

	glm::vec2 startPos = slickSam.getPyramid()->GetTileScreenPos(slickSam.getRow(), slickSam.getCol());
	glm::vec2 endPos   = slickSam.getPyramid()->GetTileScreenPos(slickSam.getFutureRow(), slickSam.getFutureCol());

	float arc = -4.f * t * (t - 1.f);
	slickSam.getOwner()->SetPosition(
		startPos.x + (endPos.x - startPos.x) * t + slickSam.getSpriteOffsetX(),
		startPos.y + (endPos.y - startPos.y) * t - arc * 30.f - slickSam.getSpriteOffsetY()
	);

	if (t >= 1.f)
	{
		slickSam.setRow(slickSam.getFutureRow());
		slickSam.setCol(slickSam.getFutureCol());
		slickSam.getOwner()->SetPosition(endPos.x + slickSam.getSpriteOffsetX(), endPos.y - slickSam.getSpriteOffsetY());

		// onTile reverses the stepped tile and returns true if Q-bert was there (Slick/Sam dies)
		if (slickSam.onTile())
			return std::make_unique<SlickSamRespawningState>(slickSam.getRespawnDuration(), 0, 0);

		return std::make_unique<SlickSamMovingState>();
	}

	return nullptr;
}
