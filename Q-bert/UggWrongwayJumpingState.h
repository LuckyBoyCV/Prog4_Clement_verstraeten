#pragma once
#include "UggWrongwayState.h"

namespace qbert
{
	// Ugg/Wrongway is mid air, playing the arc jump animation
	class UggWrongwayJumpingState final : public UggWrongwayState
	{
	public:
		std::unique_ptr<UggWrongwayState> Update(UggWrongwayComponent& uggWrongway, float deltaTime) override;
		bool isJumping() const override { return true; }
	private:
		float m_jumpTime{ 0.f };
	};
}
