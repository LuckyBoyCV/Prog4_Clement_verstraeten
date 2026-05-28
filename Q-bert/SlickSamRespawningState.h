#pragma once
#include "SlickSamState.h"

namespace qbert
{
	// Slick/Sam is hidden, waiting for its respawn delay before reappearing at a spawn tile
	class SlickSamRespawningState final : public SlickSamState
	{
	public:
		SlickSamRespawningState(float delay, int respawnRow, int respawnCol);
		std::unique_ptr<SlickSamState> Update(SlickSamComponent& slickSam, float deltaTime) override;
	private:
		float m_timer{ 0.f };
		float m_delay;
		int   m_respawnRow;
		int   m_respawnCol;
	};
}
