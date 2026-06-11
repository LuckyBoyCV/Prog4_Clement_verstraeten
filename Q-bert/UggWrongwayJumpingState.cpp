#include "UggWrongwayJumpingState.h"
#include "UggWrongwayMovingState.h"
#include "UggWrongwayComponent.h"
#include "PyramidComponent.h"
#include "GameObject.h"
#include <algorithm>

std::unique_ptr<qbert::UggWrongwayState> qbert::UggWrongwayJumpingState::Update(UggWrongwayComponent& uggWrongway, float deltaTime)
{
	m_jumpTime += deltaTime;
	float t = std::min(m_jumpTime / uggWrongway.getJumpDuration(), 1.f);

	glm::vec2 startPos = uggWrongway.getPyramid()->getTileScreenPos(uggWrongway.getRow(), uggWrongway.getCol());
	glm::vec2 endPos   = uggWrongway.getPyramid()->getTileScreenPos(uggWrongway.getFutureRow(), uggWrongway.getFutureCol());

	// Parabolic hop, peaks mid jump and 0 at both ends, lifts the sprite up to 30px off the line
	float arc = -4.f * t * (t - 1.f);
	uggWrongway.getOwner()->SetPosition(
		startPos.x + (endPos.x - startPos.x) * t + uggWrongway.getSpriteOffsetX(),
		startPos.y + (endPos.y - startPos.y) * t - arc * 30.f - uggWrongway.getSpriteOffsetY()
	);

	if (t >= 1.f)
	{
		uggWrongway.setRow(uggWrongway.getFutureRow());
		uggWrongway.setCol(uggWrongway.getFutureCol());
		uggWrongway.getOwner()->SetPosition(endPos.x + uggWrongway.getSpriteOffsetX(), endPos.y - uggWrongway.getSpriteOffsetY());

		// onTile kills Q*bert if he shares this tile, Ugg/Wrongway never flip tiles
		uggWrongway.onTile();
		return std::make_unique<UggWrongwayMovingState>();
	}

	return nullptr;
}
