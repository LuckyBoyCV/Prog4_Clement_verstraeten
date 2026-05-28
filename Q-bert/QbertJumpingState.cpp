#include "QbertJumpingState.h"
#include "QbertComponent.h"
#include "QbertIdleState.h"
#include "QbertFallingState.h"
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
			// Landed outside the pyramid  start falling
			qbert.loseLife();
			return std::make_unique<QbertFallingState>(glm::vec2{ endPos.x + 30.f, endPos.y - 30.f });
		}

		qbert.setRow(qbert.getFutureRow());
		qbert.setCol(qbert.getFutureCol());
		pyramid->StepOn(*pyramid->GetTile(qbert.getRow(), qbert.getCol()));
		qbert.getOwner()->SetPosition(endPos.x + 30.f, endPos.y - 30.f);
		return std::make_unique<QbertIdleState>();
	}

	return nullptr;
}
