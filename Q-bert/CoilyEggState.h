#pragma once
#include "CoilyState.h"

namespace qbert
{
	class CoilyComponent;

	class CoilyEggState final : public CoilyState
	{
	public:
		std::unique_ptr<CoilyState> Update(CoilyComponent& coily, float deltaTime) override;
		void onEnter(CoilyComponent& coily) override;


	private:
		float m_JumpTime{ 0.f };
		float m_JumpInterval{ 0.8f };
	};
}