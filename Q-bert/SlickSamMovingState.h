#pragma once
#include "SlickSamState.h"

namespace qbert
{
	// Slick/Sam is on a tile: checks for qbert collision, then jumps downward on an interval
	class SlickSamMovingState final : public SlickSamState
	{
	public:
		std::unique_ptr<SlickSamState> Update(SlickSamComponent& slickSam, float deltaTime) override;
	private:
		float m_jumpTime{ 0.f };
		bool  m_qbertWasJumping{ false };
	};
}
