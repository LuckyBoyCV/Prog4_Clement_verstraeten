#pragma once
#include "RedBallState.h"

namespace qbert
{
	// Ball is hidden, waiting for its respawn delay before reappearing at a spawn tile
	class RedBallRespawningState final : public RedBallState
	{
	public:
		RedBallRespawningState(float delay, int respawnRow, int respawnCol);
		std::unique_ptr<RedBallState> Update(redBallComponent& ball, float deltaTime) override;
	private:
		float m_timer{ 0.f };
		float m_delay;
		int m_respawnRow;
		int m_respawnCol;
	};
}
