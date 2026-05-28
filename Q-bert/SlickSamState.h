#pragma once
#include <memory>

namespace qbert
{
	class SlickSamComponent;

	// Abstract base for all Slick/Sam enemy states
	class SlickSamState
	{
	public:
		virtual ~SlickSamState() = default;
		// Returns the next state to transition to, or nullptr to stay in this state
		virtual std::unique_ptr<SlickSamState> Update(SlickSamComponent& slickSam, float deltaTime) = 0;
		virtual void onEnter(SlickSamComponent&) {}
		virtual void onExit(SlickSamComponent&) {}
		virtual bool isJumping() const { return false; }
	};
}
