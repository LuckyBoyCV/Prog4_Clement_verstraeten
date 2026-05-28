#pragma once
#include "CoilyState.h"
#include <memory>

namespace qbert
{
	// Plays the arc jump animation; transitions to afterLand when done
	class CoilyJumpingState final : public CoilyState
	{
	public:
		CoilyJumpingState(int destRow, int destCol, std::unique_ptr<CoilyState> afterLand);
		std::unique_ptr<CoilyState> Update(CoilyComponent& coily, float deltaTime) override;
		bool isJumping() const override { return true; }
	private:
		int   m_destRow;
		int   m_destCol;
		float m_jumpTime{ 0.f };
		static constexpr float m_jumpDuration{ 0.2f };
		std::unique_ptr<CoilyState> m_afterLand; // state to enter after landing
	};
}
