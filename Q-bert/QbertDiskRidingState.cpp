#include "QbertDiskRidingState.h"
#include "QbertComponent.h"
#include "QbertIdleState.h"
#include "PyramidComponent.h"
#include "GameObject.h"
#include <algorithm>

qbert::QbertDiskRidingState::QbertDiskRidingState(int diskIndex)
	: m_diskIndex(diskIndex)
{}

void qbert::QbertDiskRidingState::onEnter(QbertComponent& qbert)
{
	PyramidComponent* pyramid = qbert.getPyramid();
	const Disk& disk = pyramid->GetDisk(m_diskIndex);

	// +30/-30 centres the sprite the same way every other state positions Q*bert.
	m_startPos = { disk.position.x + 30.f, disk.position.y - 30.f };
	glm::vec2 apex = pyramid->GetTileScreenPos(0, 0);
	m_apexPos = { apex.x + 30.f, apex.y - 30.f };

	// Report the disc's off-pyramid coord for the whole ride: this is what lures Coily.
	qbert.setRow(disk.row);
	qbert.setCol(disk.col);
}

std::unique_ptr<qbert::QbertState> qbert::QbertDiskRidingState::Update(QbertComponent& qbert, float deltaTime)
{
	m_timer += deltaTime;
	float t = std::min(m_timer / m_rideDuration, 1.f);

	qbert.getOwner()->SetPosition(
		m_startPos.x + (m_apexPos.x - m_startPos.x) * t,
		m_startPos.y + (m_apexPos.y - m_startPos.y) * t
	);

	if (t >= 1.f)
	{
		// Arrive at the apex and consume the disc.
		qbert.setRow(0);
		qbert.setCol(0);
		qbert.getOwner()->SetPosition(m_apexPos.x, m_apexPos.y);

		PyramidComponent* pyramid = qbert.getPyramid();
		if (Tile* apex = pyramid->GetTile(0, 0))
			if (pyramid->StepOn(*apex))
				qbert.addScore(25);

		pyramid->ConsumeDisk(m_diskIndex);
		return std::make_unique<QbertIdleState>();
	}

	return nullptr;
}
