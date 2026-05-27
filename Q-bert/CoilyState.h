#pragma once
#include <memory>

namespace qbert
{
	class CoilyComponent;

	class CoilyState
	{
	public:
		virtual ~CoilyState() = default;
		virtual std::unique_ptr<CoilyState> Update(CoilyComponent& coily, float deltaTime) = 0;
		virtual void onEnter(CoilyComponent& ) {}
		virtual void onExit(CoilyComponent& ) {}
	
	};
}