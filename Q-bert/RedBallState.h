#pragma once
#include <memory>

namespace qbert
{
	class redBallComponent;

	// Abstract base for all red-ball enemy states
	class RedBallState
	{
	public:
		virtual ~RedBallState() = default;
		// Returns the next state to transition to, or nullptr to stay in this state
		virtual std::unique_ptr<RedBallState> Update(redBallComponent& ball, float deltaTime) = 0;
		virtual void onEnter(redBallComponent&) {}
		virtual void onExit(redBallComponent&) {}
		virtual bool isJumping() const { return false; }
	};
}
