#pragma once
#include "SlickSamState.h"

namespace qbert
{
	// Slick/Sam is mid-air, playing the arc jump animation
	class SlickSamJumpingState final : public SlickSamState
	{
	public:
		std::unique_ptr<SlickSamState> Update(SlickSamComponent& slickSam, float deltaTime) override;
		bool isJumping() const override { return true; }
	private:
		float m_jumpTime{ 0.f };
	};
}
