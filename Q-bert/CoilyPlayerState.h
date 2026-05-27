#pragma once
#include "CoilyState.h"

namespace qbert
{
	class coilyPlayerState final : public CoilyState
	{
	public:
		std::unique_ptr<CoilyState> Update(CoilyComponent& coily, float deltaTime) override;
		void onEnter(CoilyComponent& coily) override;
	};
}
