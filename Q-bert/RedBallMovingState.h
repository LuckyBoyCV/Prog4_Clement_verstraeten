#pragma once
#include "RedBallState.h"

namespace qbert
{
	// Ball is on a tile: checks for Q-bert collision, then jumps downward on an interval
	class RedBallMovingState final : public RedBallState
	{
	public:
		std::unique_ptr<RedBallState> Update(redBallComponent& ball, float deltaTime) override;
	private:
		float m_jumpTime{ 0.f };
		bool  m_qbertWasJumping{ false };
	};
}
