#pragma once
#include "UggWrongwayState.h"

namespace qbert
{
	// Ugg/Wrongway is on a tile: checks for qbert collision, then climbs one tile up its edge on an interval
	class UggWrongwayMovingState final : public UggWrongwayState
	{
	public:
		std::unique_ptr<UggWrongwayState> Update(UggWrongwayComponent& uggWrongway, float deltaTime) override;
	private:
		float m_jumpTime{ 0.f };
		bool  m_qbertWasJumping{ false };
	};
}
