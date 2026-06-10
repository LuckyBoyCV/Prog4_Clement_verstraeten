#include "CoilyJumpingState.h"
#include "CoilyComponent.h"
#include "PyramidComponent.h"
#include "GameObject.h"
#include <algorithm>

qbert::CoilyJumpingState::CoilyJumpingState(int destRow, int destCol, std::unique_ptr<CoilyState> afterLand)
	: m_destRow(destRow)
	, m_destCol(destCol)
	, m_afterLand(std::move(afterLand))
{}

std::unique_ptr<qbert::CoilyState> qbert::CoilyJumpingState::Update(CoilyComponent& coily, float deltaTime)
{
	m_jumpTime += deltaTime;
	float t = std::min(m_jumpTime / m_jumpDuration, 1.f);

	glm::vec2 startPos = coily.getPyramid()->GetTileScreenPos(coily.getRow(), coily.getCol());
	glm::vec2 endPos   = coily.getPyramid()->GetTileScreenPos(m_destRow, m_destCol);

	// parabolic arc: peaks at t=0.5
	float arc = -4.f * t * (t - 1.f);
	coily.getOwner()->SetPosition(
		startPos.x + (endPos.x - startPos.x) * t + 30.f,
		startPos.y + (endPos.y - startPos.y) * t - arc * 30.f + coily.getSpriteOffsetY()
	);

	if (t >= 1.f)
	{
		// Landed off the pyramid (and not onto a disc) -> Coily was lured over the edge.
		PyramidComponent* pyramid = coily.getPyramid();
		if (!pyramid->GetTile(m_destRow, m_destCol) && pyramid->GetDiskIndexAt(m_destRow, m_destCol) < 0)
		{
			coily.die();        // awards the player and queues Coily's respawn
			return nullptr;     // die() drives the next state; skip the normal landing
		}

		coily.setRow(m_destRow);
		coily.setCol(m_destCol);
		coily.onTile(); // kill Q*bert if they share a tile
		return std::move(m_afterLand);
	}

	return nullptr;
}
