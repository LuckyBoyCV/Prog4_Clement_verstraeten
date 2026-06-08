#pragma once
#include "Component.h"
#include "Observer.h"

namespace qbert
{
	// Keeps a TextComponent (on the same GameObject) in sync with a Q*bert's
	// score. Observes that Q*bert's subject and refreshes when points are scored.
	class ScoreDisplayComponent final : public dae::Component, public dae::Observer
	{
	public:
		explicit ScoreDisplayComponent(dae::GameObject* pOwner);
		~ScoreDisplayComponent() override = default;

		ScoreDisplayComponent(const ScoreDisplayComponent&) = delete;
		ScoreDisplayComponent(ScoreDisplayComponent&&) = delete;
		ScoreDisplayComponent& operator=(const ScoreDisplayComponent&) = delete;
		ScoreDisplayComponent& operator=(ScoreDisplayComponent&&) = delete;

		void OnNotify(dae::GameEvent event, dae::GameObject* pOwner) override;
	};
}
