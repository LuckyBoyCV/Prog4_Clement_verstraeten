#pragma once
#include "CoilyState.h"

namespace qbert
{
	// Coily is off-screen waiting to re-enter; spawns as egg when the delay runs out
	class CoilyRespawningState final : public CoilyState
	{
	public:
		CoilyRespawningState(float delay, int respawnRow, int respawnCol);
		std::unique_ptr<CoilyState> Update(CoilyComponent& coily, float deltaTime) override;
		bool isDangerous() const override { return false; } // off-screen waiting to hatch, can't kill
	private:
		float m_delay;
		float m_timer{ 0.f };
		int   m_respawnRow;
		int   m_respawnCol;
	};
}
