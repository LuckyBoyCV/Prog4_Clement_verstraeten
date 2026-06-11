#pragma once
#include <memory>

namespace qbert
{
	class UggWrongwayComponent;

	// Abstract base for all Ugg/Wrongway enemy states
	class UggWrongwayState
	{
	public:
		virtual ~UggWrongwayState() = default;
		// Returns the next state to transition to, or nullptr to stay in this state
		virtual std::unique_ptr<UggWrongwayState> Update(UggWrongwayComponent& uggWrongway, float deltaTime) = 0;
		virtual void onEnter(UggWrongwayComponent&) {}
		virtual void onExit(UggWrongwayComponent&) {}
		virtual bool isJumping() const { return false; }
	};
}
