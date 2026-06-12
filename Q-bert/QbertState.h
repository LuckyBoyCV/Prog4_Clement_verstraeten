#pragma once
#include <memory>

namespace qbert
{
	class QbertComponent;

	// Abstract base for all qbert player states
	class QbertState
	{
	public:
		virtual ~QbertState() = default;
		// Returns the next state to transition to, or nullptr to stay in this state
		virtual std::unique_ptr<QbertState> Update(QbertComponent& qbert, float deltaTime) = 0;
		virtual void onEnter(QbertComponent&) {}
		virtual void onExit(QbertComponent&) {}
		virtual bool isJumping() const { return false; }
		virtual bool isFalling() const { return false; }
		virtual bool isRidingDisk() const { return false; }
	};
}
