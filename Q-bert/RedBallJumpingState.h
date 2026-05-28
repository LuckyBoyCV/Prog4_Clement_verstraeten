#pragma once
#include "RedBallState.h"

namespace qbert
{
	// Ball is mid-air, playing the arc jump animation
	class RedBallJumpingState final : public RedBallState
	{
	public:
		std::unique_ptr<RedBallState> Update(redBallComponent& ball, float deltaTime) override;
		bool isJumping() const override { return true; }
	private:
		float m_jumpTime{ 0.f };
	};
}
