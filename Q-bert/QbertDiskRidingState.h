#pragma once
#include "QbertState.h"
#include <glm/vec2.hpp>

namespace qbert
{
	// qbert jumped onto a flying disc: rides it from the disc up to the apex, then drops
	// back to idle at the top. While riding, qbert's logical (row,col) is the disc's
	// off-pyramid coord, which lures a chasing Coily off the edge to its death.
	class QbertDiskRidingState final : public QbertState
	{
	public:
		explicit QbertDiskRidingState(int diskIndex);
		std::unique_ptr<QbertState> Update(QbertComponent& qbert, float deltaTime) override;
		void onEnter(QbertComponent& qbert) override;
		bool isRidingDisk() const override { return true; }
	private:
		int m_diskIndex;
		float m_timer{ 0.f };
		glm::vec2 m_startPos{};
		glm::vec2 m_apexPos{};
		static constexpr float m_rideDuration{ 1.5f };
	};
}
