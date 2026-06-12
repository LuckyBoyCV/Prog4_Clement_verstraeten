#pragma once
#include "QbertState.h"
#include <glm/vec2.hpp>

namespace qbert
{
	// qbert fell off the pyramid — plays fall animation then triggers respawn
	class QbertFallingState final : public QbertState
	{
	public:
		explicit QbertFallingState(glm::vec2 fallStartPos);
		std::unique_ptr<QbertState> Update(QbertComponent& qbert, float deltaTime) override;
		bool isFalling() const override { return true; }
	private:
		float m_timer{ 0.f };
		glm::vec2 m_fallStartPos;
	};
}
