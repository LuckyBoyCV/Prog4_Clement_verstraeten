#pragma once
#include "Component.h"
#include "Observer.h"

namespace qbert
{
	// Keeps a TextComponent (on the same GameObject) in sync with a qbert's
	// remaining lives. Observes that qbert's subject and refreshes on life loss.
	class LivesDisplayComponent final : public dae::Component, public dae::Observer
	{
	public:
		explicit LivesDisplayComponent(dae::GameObject* pOwner);
		~LivesDisplayComponent() override = default;

		LivesDisplayComponent(const LivesDisplayComponent&) = delete;
		LivesDisplayComponent(LivesDisplayComponent&&) = delete;
		LivesDisplayComponent& operator=(const LivesDisplayComponent&) = delete;
		LivesDisplayComponent& operator=(LivesDisplayComponent&&) = delete;

		void OnNotify(dae::GameEvent event, dae::GameObject* pOwner) override;
	};
}
