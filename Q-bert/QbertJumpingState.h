#pragma once
#include "QbertState.h"

namespace qbert
{
	// qbert is mid-air, playing the arc jump animation
	class QbertJumpingState final : public QbertState
	{
	public:
		std::unique_ptr<QbertState> Update(QbertComponent& qbert, float deltaTime) override;
		bool isJumping() const override { return true; }
	private:
		float m_jumpTime{ 0.f };
	};
}
