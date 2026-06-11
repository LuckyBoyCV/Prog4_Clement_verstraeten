#pragma once
#include "UggWrongwayState.h"

namespace qbert
{
	// Ugg/Wrongway is hidden, waiting for its respawn delay before reappearing at its edge bottom corner
	class UggWrongwayRespawningState final : public UggWrongwayState
	{
	public:
		UggWrongwayRespawningState(float delay, int respawnRow, int respawnCol);
		std::unique_ptr<UggWrongwayState> Update(UggWrongwayComponent& uggWrongway, float deltaTime) override;
	private:
		float m_timer{ 0.f };
		float m_delay;
		int   m_respawnRow;
		int   m_respawnCol;
	};
}
