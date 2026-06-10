#include "QbertJumpingState.h"
#include "QbertComponent.h"
#include "QbertIdleState.h"
#include "QbertFallingState.h"
#include "QbertDiskRidingState.h"
#include "PyramidComponent.h"
#include "GameObject.h"
#include <algorithm>

std::unique_ptr<qbert::QbertState> qbert::QbertJumpingState::Update(QbertComponent& qbert, float deltaTime)
{
	m_jumpTime += deltaTime;
	float t = std::min(m_jumpTime / qbert.getJumpDuration(), 1.f);

	PyramidComponent* pyramid = qbert.getPyramid();
	glm::vec2 startPos = pyramid->GetTileScreenPos(qbert.getRow(), qbert.getCol());
	glm::vec2 endPos   = pyramid->GetTileScreenPos(qbert.getFutureRow(), qbert.getFutureCol());

	float arc = -4.f * t * (t - 1.f);
	qbert.getOwner()->SetPosition(
		startPos.x + (endPos.x - startPos.x) * t + 30.f,
		startPos.y + (endPos.y - startPos.y) * t - arc * 30.f - 30.f
	);

	if (t >= 1.f)
	{
		if (!pyramid->GetTile(qbert.getFutureRow(), qbert.getFutureCol()))
		{
			// Off the pyramid: if a flying disc is there, ride it up instead of dying.
			int disk = pyramid->GetDiskIndexAt(qbert.getFutureRow(), qbert.getFutureCol());
			if (disk >= 0)
				return std::make_unique<QbertDiskRidingState>(disk);

			// Otherwise it's a genuine fall off the edge.
			qbert.loseLife();
			return std::make_unique<QbertFallingState>(glm::vec2{ endPos.x + 30.f, endPos.y - 30.f });
		}

		qbert.setRow(qbert.getFutureRow());
		qbert.setCol(qbert.getFutureCol());

		// StepOn scores only when the tile advances toward its target colour, so
		// re-hopping a finished tile (or a level-3 revert) earns nothing.
		Tile* landed = pyramid->GetTile(qbert.getRow(), qbert.getCol());
		if (pyramid->StepOn(*landed))
			qbert.addScore(25);

		qbert.getOwner()->SetPosition(endPos.x + 30.f, endPos.y - 30.f);
		return std::make_unique<QbertIdleState>();
	}

	return nullptr;
}
